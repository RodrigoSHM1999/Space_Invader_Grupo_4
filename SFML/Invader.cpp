#include "Invader.h"

void Invader::reset(const unsigned x, const unsigned y)
{
    this->move_dir = 1; // Dirección inicial de movimiento del invasor (derecha)
    this->is_dead = false; // Establece el estado de muerte del invasor a falso
    this->is_exploding = false; // Establece el estado de explosión del invasor a falso
    this->frame_on = 1; // Establece el fotograma actual de la animación a 1
    this->death_tick = 0; // Establece el contador de tick de muerte a 0
    this->lasers_on_screen = 0; // Establece la cantidad de láseres del invasor en pantalla a 0

    this->sprite.setTexture(*(this->frame1_txtr), true); // Establece la textura inicial del invasor (fotograma 1)
    this->sprite.setColor(sf::Color::White); // En caso de que se haya vuelto verde en la oleada anterior.
    this->sprite.setPosition(x, y); // Establece la posición del invasor en las coordenadas dadas
}

void Invader::flipFrame()
{
    if (this->frame_on == 1)
    {
        this->sprite.setTexture(*(this->frame2_txtr), true); // Cambia a la textura del fotograma 2
        this->frame_on = 2; // Establece el fotograma actual a 2
    }
    else
    {
        this->sprite.setTexture(*(this->frame1_txtr), true); // Cambia a la textura del fotograma 1
        this->frame_on = 1; // Establece el fotograma actual a 1
    }
}

Invader::Invader(Textures& textures, const InvaderType type) : type(type), move_dir(1), is_dead(false), is_exploding(false), frame_on(1), death_tick(0), lasers_on_screen(0)
{
    // Carga diferentes texturas y establece diferentes valores de puntuación según el tipo de invasor
    switch (this->type)
    {
    case Invader::LARGE:
        this->frame1_txtr = &(textures.LARGE_INV_1);
        this->frame2_txtr = &(textures.LARGE_INV_2);
        this->score_value = 10;
        break;

    case Invader::MEDIUM:
        this->frame1_txtr = &(textures.MEDIUM_INV_1);
        this->frame2_txtr = &(textures.MEDIUM_INV_2);
        this->score_value = 20;
        break;

    case Invader::SMALL:
        this->frame1_txtr = &(textures.SMALL_INV_1);
        this->frame2_txtr = &(textures.SMALL_INV_2);
        this->score_value = 30;
        break;

    default:
        break;
    }

    this->frame3_txtr = &(textures.INVADER_DEATH);
    this->sprite.setTexture(*(this->frame1_txtr), true);// Establece la textura inicial del invasor (fotograma 1)

    // Coloca el punto de origen en el centro del invasor.
    this->sprite.setOrigin(this->sprite.getGlobalBounds().width / 2, this->sprite.getGlobalBounds().width / 2);
}

void Invader::die()
{
    this->sprite.setTexture(*(this->frame3_txtr), true); // Establece la textura de muerte del invasor
    this->is_exploding = true; // Establece el estado de explosión del invasor a verdadero
    this->is_dead = true; // Establece el estado de muerte del invasor a verdadero
}

void Invader::move()
{
    if (this->isDead())
        return;

    this->sprite.move(this->SPEED * this->move_dir, 0); // Mueve el invasor según su dirección de movimiento
    this->flipFrame(); // Cambia al siguiente fotograma de la animación
}

void Invader::dropDown(const unsigned shield_line)
{
    if (this->isDead())
        return;

    this->sprite.move(0, this->HEIGHT); // Desplaza el invasor hacia abajo
    this->flipFrame(); // Cambia al siguiente fotograma de la animación

    if (this->sprite.getPosition().y >= shield_line)
        this->sprite.setColor(sf::Color::Green); // Cambia el color del invasor a verde si alcanza la línea de escudo
}

void Invader::reverseDir()
{
    this->move_dir *= -1; // Invierte la dirección de movimiento del invasor
}

bool Invader::checkHitEdge(const int screenw)
{
    if (this->isDead())
        return false;

    int x = this->sprite.getPosition().x;
    int half_width = (this->sprite.getTexture()->getSize().x) / 2;

    // +/- 10 para evitar que se salga ligeramente de la pantalla
    if ((x >= (screenw - half_width - 10) && this->move_dir == 1) || (x <= (half_width + 10) && this->move_dir == -1))
        return true; // Ha alcanzado el borde de la pantalla

    return false;// No ha alcanzado el borde de la pantalla
}

void Invader::incDeathTick()
{
    ++this->death_tick; // Incrementa el contador de tick de muerte
    if (this->death_tick >= this->DEATH_TICK_MAX)
        this->is_exploding = false;  // Establece el estado de explosión a falso una vez que alcanza el máximo de ticks de muerte
}
