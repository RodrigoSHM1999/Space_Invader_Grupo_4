#include "globals.h"
#include "misc.h"
#include "InvaderFormation.h"
#include "CoreCannon.h"

void InvaderFormation::reset(unsigned wave_on)
{
    this->move_tick = 0;
    this->move_tick_max = MOVE_TICK_MAX_START;
    this->move_tick_change = MOVE_TICK_CHANGE_START;
    this->step_on = 1;
    this->num_killed = 0;
    this->has_hit_edge = false;
    this->shot_chance = this->SHOT_CHANCE_START;

    this->removeLasers();

    for (unsigned i = 0; i < this->invaders.size(); ++i)
    {
        for (unsigned j = 0; j < this->invaders[i].size(); ++j)
            this->invaders[i][j]->reset(this->STARTX + (j * this->XGAP), (this->STARTY + (wave_on * Invader::getHeight())) + (i * this->YGAP));
    }
}

void InvaderFormation::incDeathTick()
{
    for (auto& invader_row : this->invaders)
    {
        for (auto& invader : invader_row)
        {
            if (invader->isExploding())
                invader->incDeathTick();
        }
    }
}

void InvaderFormation::playStepSound()
{
    // Los invasores hacen 4 sonidos en un bucle en cada paso.
    switch (this->step_on)
    {
    case 1:
        this->step1.play();
        break;

    case 2:
        this->step2.play();
        break;

    case 3:
        this->step3.play();
        break;

    case 4:
        this->step4.play();
        break;

    default:
        break;
    }


    ++this->step_on;
    if (this->step_on > 4)
        this->step_on = 1;
}

void InvaderFormation::increaseFire(int amount)
{
    this->shot_chance -= amount;
    if (this->shot_chance < 10)
        shot_chance = 10;
}

bool InvaderFormation::toShootOrNah() const
{
    // Número arbitrario, hay 1/disparo_posibilidad de devolver verdadero.
    return Misc::random(1, this->shot_chance) == 1;
}

void InvaderFormation::shootLaser(unsigned x, unsigned y, const bool will_hurt, Invader& owner)
{
    // Los láseres normales tienen una probabilidad ligeramente mayor de ser creados.
    unsigned dice_roll = Misc::random(1, 10);
    if (dice_roll <= 6)
        this->lasers.push_back(new NormalInvaderLaser(x, y, this->SHIELD_LINE, will_hurt, owner));
    else
        this->lasers.push_back(new PoweredInvaderLaser(x, y, this->SHIELD_LINE, will_hurt, owner));
}

void InvaderFormation::handleCollisions(Explosions& explosions, PlayerLaser& player_laser)
{
    for (unsigned i = 0; i < this->lasers.size(); ++i)
    {
        InvaderLaser* laser = this->lasers[i];

        // Comprueba la colisión con la tierra
        if (laser->checkCollide(earth.getSprite().getGlobalBounds()))
        {
            earth.destroy(laser->getX());
            laser->setHit();
            explosions.newExplosion(sf::Color::Green, laser->getX(), laser->getY());
        }

        // Comprueba la colisión con el láser del jugador
        if (laser->checkCollide(player_laser.getShape().getGlobalBounds()))
        {
            // El láser del jugador siempre se destruye.
            explosions.newExplosion(sf::Color::White, laser->getX(), laser->getY());
            player_laser.stop();

            // Los láseres normales tienen una probabilidad de 1 en 3 de ser destruidos si son golpeados por el láser del jugador.
            if (laser->getType() == InvaderLaser::LaserType::NORMAL && (Misc::random(1, 3) == 2))
                laser->setHit();
        }
    }
}

void InvaderFormation::shootLasers()
{
    for (unsigned i = 0; i < this->invaders.size(); ++i)
    {
        for (unsigned j = 0; j < this->invaders[i].size(); ++j)
        {
            Invader* invader = this->invaders[i][j];

            // Solo dispara si Invader está vivo y gana el premio, si ya tiene menos de 3 láseres en la pantalla.
            if (!invader->isDead() && invader->getLasersOnScreen() < 3 && this->toShootOrNah())
            {
                bool shoot = true;

                // Solo dispara un láser si no hay un Invasor debajo del Invasor actual.
                for (unsigned k = i + 1; k <= this->invaders.size(); ++k)
                {
                    // Verifique el valor de k contra el tamaño porque queremos que la fila inferior se dispare
                    // pero el circuito corto nos salva de salirnos de los límites.
                    if (k != this->invaders.size() && !this->invaders[k][j]->isDead())
                    {
                        shoot = false;
                        break; // No es necesario seguir comprobando.
                    }
                }

                if (shoot)
                {
                    sf::Vector2f invader_pos = invader->getSprite().getPosition();
                    // Si el Invasor está justo encima del jugador, su láser no le hará daño.
                    this->shootLaser(invader_pos.x, invader_pos.y, invader->getSprite().getPosition().y < (this->INVADE_LINE - 40), *invader);
                }
            }
        }
    }
}

void InvaderFormation::moveLasers(Explosions& explosions, PlayerLaser& player_laser)
{
    this->handleCollisions(explosions, player_laser);
    for (auto& laser : this->lasers)
        laser->move();
}

void InvaderFormation::removeHitLasers()
{
    for (unsigned i = 0; i < this->lasers.size(); ++i)
    {
        InvaderLaser* laser = lasers[i];
        if (laser->isHit())
        {
            delete laser;
            this->lasers.erase(lasers.begin() + i);
        }
    }
}

bool InvaderFormation::move(CoreCannon& cannon, PlayerLaser& player_laser, UFO& ufo, LivesDisplay& lives_disp, Explosions& explosions)
{
    // El último Invasor se mueve más rápido hacia la izquierda que hacia la derecha.
    if (this->getNumKilled() == (this->getTotal() - 1))
    {
        if (this->invaders[0][0]->getMoveDir() == 1)
            this->move_tick_max = 2;
        else
            this->move_tick_max = 1;
    }

    this->incDeathTick(); // move() se llama en cada fotograma.
    ++this->move_tick;
    if (this->move_tick == this->move_tick_max)
    {
        if (this->has_hit_edge)
        {
            this->shift();
            this->has_hit_edge = false;
        }
        else
        {
            for (auto& invader_row : this->invaders)
            {
                for (auto& invader : invader_row)
                {
                    // Si Invader ha llegado lo suficientemente lejos, se acabó el juego.
                    if (invader->getSprite().getPosition().y >= this->INVADE_LINE)
                    {
                        cannon.die(*this, player_laser, ufo, explosions);
                        lives_disp.setLives(0);
                    }

                    invader->move();
                    if (invader->checkHitEdge(Globals::SCREEN_WIDTH))
                        this->has_hit_edge = true;
                }
            }
        }

        this->move_tick = 0;
        this->playStepSound();

        return true;
    }

    return false;
}

void InvaderFormation::shift()
{
    // No dejan que los invasores se muevan demasiado rápidos.
    if (this->move_tick_max > 2)
        --this->move_tick_max;

    for (auto& invader_row : this->invaders)
    {
        for (auto& invader : invader_row)
        {
            invader->dropDown(this->SHIELD_LINE);
            invader->reverseDir();
        }
    }

    // Aumenta la tasa de disparo de los Invasores.
    this->increaseFire(10);
}

void InvaderFormation::checkHit(PlayerLaser& laser, unsigned& game_score)
{
    for (auto& invader_row : this->invaders)
    {
        for (auto& invader : invader_row)
        {
            if (!invader->isDead() && invader->getSprite().getGlobalBounds().intersects(laser.getShape().getGlobalBounds()))
            {
                invader->die();

                // Aumenta la puntuación del juego principal, definida originalmente en main()
                game_score += invader->getScoreValue();

                ++this->num_killed;

                // Si es el último invasor, se mueve lo más rápido posible.
                if (this->getNumKilled() == (this->getTotal() - 1))
                {
                    this->move_tick = 0;
                    this->move_tick_max = 1;
                }

                // La formación se acelera por cada 6 invasores asesinados.
                else if (!(this->num_killed % 6) && this->move_tick_max > 1)
                {
                    this->move_tick_max -= 5;

                    // Esto es para mantener la formación en movimiento.
                    if (this->move_tick >= this->move_tick_max)
                        this->move_tick = this->move_tick_max - 1;

                    // No deja que los invasores se muevan demasiado rápido
                    if (this->move_tick_max < 2)
                        this->move_tick_max = 2;
                }

                // También aumenta la velocidad de disparo.
                this->increaseFire(17);

                this->death_snd.play();
                laser.stop();

                return; // Solo se puede golpear 1 Invasor a la vez.
            }
        }

    }
}

void InvaderFormation::updateLasers(Explosions& explosions, PlayerLaser& player_laser)
{
    this->removeHitLasers();
    this->handleCollisions(explosions, player_laser);
    this->moveLasers(explosions, player_laser);
    this->shootLasers();
}

void InvaderFormation::removeLasers()
{
    // Destruir láseres
    for (auto& laser : this->lasers)
        delete laser;
    this->lasers.clear();
}

void InvaderFormation::drawLasers()
{
    for (auto& laser : this->lasers)
        laser->draw(this->window);
}

InvaderFormation::InvaderFormation(sf::RenderWindow& window, Textures& textures, Earth& earth, SoundFx& step1, SoundFx& step2, SoundFx& step3, SoundFx& step4, SoundFx& death_snd) : window(window), earth(earth), move_tick(0), move_tick_max(MOVE_TICK_MAX_START), move_tick_change(MOVE_TICK_CHANGE_START), step_on(1), num_killed(0), has_hit_edge(false), shot_chance(SHOT_CHANCE_START), step1(step1), step2(step2), step3(step3), step4(step4), death_snd(death_snd)
{
    // Vector para cada fila en la formación
    InvaderRow small_invaders;
    InvaderRow medium_invaders1;
    InvaderRow medium_invaders2;
    InvaderRow large_invaders1;
    InvaderRow large_invaders2;

    for (unsigned i = 1; i <= this->ROWS; ++i)
    {
        small_invaders.push_back(new Invader(textures, Invader::SMALL));
        medium_invaders1.push_back(new Invader(textures, Invader::MEDIUM));
        medium_invaders2.push_back(new Invader(textures, Invader::MEDIUM));
        large_invaders1.push_back(new Invader(textures, Invader::LARGE));
        large_invaders2.push_back(new Invader(textures, Invader::LARGE));
    }

    // Agrega a cada fila al vector principal
    this->invaders.push_back(small_invaders);
    this->invaders.push_back(medium_invaders1);
    this->invaders.push_back(medium_invaders2);
    this->invaders.push_back(large_invaders1);
    this->invaders.push_back(large_invaders2);

    this->reset();
}

InvaderFormation::~InvaderFormation()
{
    // Destruir invasores
    for (auto& invader_row : this->invaders)
    {
        for (auto& invader : invader_row)
            delete invader;

        invader_row.clear();
    }
    this->invaders.clear();

    this->removeLasers();
}

void InvaderFormation::update(PlayerLaser& laser, CoreCannon& cannon, PlayerLaser& player_laser, UFO& ufo, LivesDisplay& lives_disp, Explosions& explosions, unsigned& game_score)
{
    this->move(cannon, player_laser, ufo, lives_disp, explosions);
    this->checkHit(laser, game_score);
    this->updateLasers(explosions, player_laser);
}

void InvaderFormation::draw(int amount)
{
    // Dibujar filas de abajo hacia arriba
    // El decremento está en el cuerpo del ciclo porque debe ocurrir antes de la primera iteración.
    for (auto iter = this->invaders.end(); iter != this->invaders.begin(); /* intentionally empty */)
    {
        --iter; //end() no se puede desreferenciar, por lo tanto, disminuya inmediatamente el iterador.

        for (auto& invader : *iter)
        {
            if (!invader->isDead() || (invader->isDead() && invader->isExploding()))
            {
                this->window.draw(invader->getSprite());
                --amount;

                // El valor predeterminado para la cantidad es -1, por lo que si se deja en el valor predeterminado,
                // esto nunca se ejecutará, lo que significa que se dibujarán todos los Invasores.
                if (amount == 0)
                    return;
            }
        }
    }
}

void InvaderFormation::setShieldLine(const unsigned shield_line)
{
    this->SHIELD_LINE = shield_line;
}
