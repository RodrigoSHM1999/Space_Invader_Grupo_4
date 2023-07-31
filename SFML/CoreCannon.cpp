#include "globals.h"
#include "game.h"
#include "CoreCannon.h"

// La función "flipFrame" cambia el frame de animación de la nave del jugador cuando es destruida.
// Alterna entre dos texturas de animación para simular el efecto de explosión.
void CoreCannon::flipFrame()
{
    if (this->frame_on == 1)
    {
        this->frame_on = 2;
        this->sprite.setTexture(this->textures.CORECANNON_DEATH_2, true);
    }
    else
    {
        this->frame_on = 1;
        this->sprite.setTexture(this->textures.CORECANNON_DEATH_1, true);
    }
}

// La función "reset" restablece la nave del jugador a su estado inicial cuando es destruida.
// Reinicia los contadores, cambia el frame a la textura original de la nave y coloca el punto de origen en el centro de la nave.
void CoreCannon::reset()
{
    this->death_tick = this->DEATH_TICK_MAX;
    this->frame_tick = this->FRAME_TICK_MAX;
    this->frame_on = 1;
    this->hit = false;

    this->sprite.setTexture(this->textures.CORECANNON, true);
    this->sprite.setPosition(this->START_X, this->Y);

    // Coloca el punto de origen en el centro del cañón.
    this->sprite.setOrigin(this->sprite.getTexture()->getSize().x / 2, this->sprite.getTexture()->getSize().y / 2);

    // Si el estado del juego no es de configuración de oleada, juego terminado o menú, establece el estado de juego como "PLAY".
    if (Globals::GAME_STATE != Globals::States::WAVE_SETUP && Globals::GAME_STATE != Globals::States::GAME_OVER && Globals::GAME_STATE != Globals::States::MENU)
        Globals::GAME_STATE = Globals::States::PLAY;
}

// El constructor de la clase "CoreCannon" inicializa los miembros de la clase, como las texturas, sonidos, posición inicial y variables de estado de la nave.
CoreCannon::CoreCannon(Textures& textures, SoundFx& killed_sound, const int startx) : killed_sound(killed_sound), START_X(startx), textures(textures), frame_on(1), hit(false), death_tick(DEATH_TICK_MAX), frame_tick(FRAME_TICK_MAX)
{
    this->reset();
}

// La función "move" mueve la nave del jugador hacia la izquierda o la derecha según la dirección proporcionada.
// El movimiento está limitado para que la nave no se salga ligeramente de la pantalla.
void CoreCannon::move(const int dir)
{
    // +/- 10 porque detiene al cañón para que no se salga ligeramente de la pantalla.
    if (!this->hit && ((dir == 1 && (this->getX() < (Globals::SCREEN_WIDTH - (this->getWidth() / 2) - 10)))
        || (dir == -1 && (this->getX() > (0 + (this->getWidth() / 2) + 10)))))
        this->sprite.move(this->SPEED * dir, 0);
}

// La función "die" se llama cuando la nave del jugador es golpeada por un láser invasor.
// Establece la nave como "golpeada" y cambia su textura de animación a la primera parte de la explosión.
// También reproduce un sonido de muerte y maneja la lógica del juego cuando el jugador muere.
void CoreCannon::die(InvaderFormation& invaders, PlayerLaser& player_laser, UFO& ufo, Explosions& explosions)
{
    this->hit = true;
    this->sprite.setTexture(this->textures.CORECANNON_DEATH_1, true);
    this->killed_sound.play();

    Game::handle_player_kill(invaders, player_laser, ufo, explosions);
}

// La función "handleHit" verifica si la nave del jugador es golpeada por algún láser invasor.
// Llama al método "die" si corresponde, lo que inicia el proceso de muerte de la nave.
void CoreCannon::handleHit(InvaderFormation& invaders, PlayerLaser& player_laser, UFO& ufo, Explosions& explosions)
{
    for (auto& laser : invaders.getLasers())
    {
        if (!this->hit && laser->willHurt() && (laser->checkCollide(this->sprite.getGlobalBounds())))
        {
            this->die(invaders, player_laser, ufo, explosions);
            return;
        }
    }
}

// La función "update" actualiza el estado de la nave del jugador en cada fotograma del juego.
// Si la nave está "golpeada", se ejecuta una animación de muerte y se disminuye un contador para la animación.
// Cuando el contador llega a cero, se quita una vida al jugador y se restablece la nave.
void CoreCannon::update(InvaderFormation& invaders, PlayerLaser& player_laser, UFO& ufo, LivesDisplay& lives_disp, Explosions& explosions)
{
    this->handleHit(invaders, player_laser, ufo, explosions);
    if (this->hit)
    {
        --this->death_tick;
        if (this->death_tick <= 0)
        {
            lives_disp.removeLife();
            ufo.pause(); // Reanuda el OVNI...
            this->reset();
        }

        --this->frame_tick;
        if (this->frame_tick <= 0)
        {
            this->flipFrame();
            this->frame_tick = this->FRAME_TICK_MAX;
        }
    }
}
