#include "NormalInvaderLaser.h"

// Método privado para animar el movimiento ascendente y descendente de la parte horizontal del láser.
void NormalInvaderLaser::moveHPart()
{
    // Obtener las posiciones actuales de las partes horizontal y vertical del láser.
    unsigned h_part_y = this->h_part.getPosition().y;
    unsigned v_part_y = this->v_part.getPosition().y;

    // Cambiar la dirección de movimiento de la parte horizontal si alcanza los límites superior o inferior.
    if ((h_part_y >= (v_part_y + this->HEIGHT - this->THICKNESS)) && this->h_part_movedir == 1)
        this->h_part_movedir = -1;
    else if ((h_part_y <= (v_part_y)) && this->h_part_movedir == -1)
        this->h_part_movedir = 1;

    // Si se está moviendo hacia abajo, es necesario moverse ligeramente más rápido que la parte vertical.
    // Permanece inmóvil si se está moviendo hacia arriba (ilusión creada por la parte vertical que sigue bajando).
    if (this->h_part_movedir == 1)
        this->h_part.move(0, this->SPEED * this->h_part_moverate);
}

// Constructor que inicializa las partes del láser normal y establece su posición inicial.
NormalInvaderLaser::NormalInvaderLaser(const unsigned x, const unsigned y, const unsigned shield_line, const bool will_hurt, Invader& owner) : InvaderLaser(LaserType::NORMAL, 5, 15, shield_line, will_hurt, owner), h_part_movedir(1)
{
    // Crear las formas que componen el láser normal.
    this->v_part = sf::RectangleShape(sf::Vector2f(this->THICKNESS, this->HEIGHT));
    this->h_part = sf::RectangleShape(sf::Vector2f(this->WIDTH, this->THICKNESS));

    // Establecer el color de las partes del láser a blanco.
    this->v_part.setFillColor(sf::Color::White);
    this->h_part.setFillColor(sf::Color::White);

    // Establecer la posición inicial de las partes del láser.
    this->v_part.setPosition(x + ((this->WIDTH / 2) - 1), y);
    this->h_part.setPosition(x, y);
}

// Método para obtener la posición en el eje X del láser.
unsigned NormalInvaderLaser::getX() const
{
    return this->v_part.getPosition().x;
}

// Método para obtener la posición en el eje Y del láser.
unsigned NormalInvaderLaser::getY() const
{
    return this->v_part.getPosition().y;
}

// Método para mover el láser hacia abajo (posición inferior de la pantalla) y animar la parte horizontal.
void NormalInvaderLaser::move()
{
    // Mover las partes vertical y horizontal del láser hacia abajo.
    this->v_part.move(0, this->SPEED);
    this->moveHPart();

    // Cambiar el color a verde una vez que el láser pasa los escudos.
    if (this->v_part.getPosition().y >= this->SHIELD_LINE)
    {
        this->v_part.setFillColor(sf::Color::Green);
        this->h_part.setFillColor(sf::Color::Green);
    }
}

// Método para dibujar las partes del láser en la ventana proporcionada.
void NormalInvaderLaser::draw(sf::RenderWindow& window)
{
    window.draw(this->v_part);
    window.draw(this->h_part);
}

// Método para verificar si el láser colisiona con una posición específica (indicada por las coordenadas x e y).
bool NormalInvaderLaser::checkCollide(const unsigned x, const unsigned y) const
{
    // Verificar si las partes vertical u horizontal del láser contienen las coordenadas x e y proporcionadas.
    if (this->v_part.getGlobalBounds().contains(x, y) || this->h_part.getGlobalBounds().contains(x, y))
        return true;

    return false;
}

// Método para verificar si el láser colisiona con un rectángulo específico, que se proporciona como argumento.
bool NormalInvaderLaser::checkCollide(const sf::FloatRect rect) const
{
    // Verificar si las partes vertical u horizontal del láser intersectan con el rectángulo proporcionado.
    if (rect.intersects(this->v_part.getGlobalBounds()) || rect.intersects(this->h_part.getGlobalBounds()))
        return true;

    return false;
}
