#include "PoweredInvaderLaser.h"

// La clase "PoweredInvaderLaser" es una subclase de "InvaderLaser" y representa un láser motorizado que se mueve más rápido y causa más daño a los escudos.

// El constructor de "PoweredInvaderLaser" inicializa los miembros de la clase, incluidas las partes que componen el láser, su posición y su orientación inicial.
PoweredInvaderLaser::PoweredInvaderLaser(const unsigned x, const unsigned y, const unsigned shield_line, const bool will_hurt, Invader& owner) : InvaderLaser(LaserType::POWERED, 6, 20, shield_line, will_hurt, owner), flip_tick(FLIP_TICK_MAX)
{
    // Se crean cuatro partes rectangulares que forman el rayo del láser.
    this->part1 = sf::RectangleShape(sf::Vector2f(this->WIDTH, this->HEIGHT));
    this->part1.setOrigin(this->WIDTH / 2, this->HEIGHT / 2);
    this->part1.setFillColor(sf::Color::White);

    // Todas las partes del rayo inicialmente son iguales en su mayoría.
    this->part2 = this->part1;
    this->part3 = this->part2;
    this->part4 = this->part3;

    // Se posicionan las partes del rayo según su orientación.
    this->part1.setPosition(x, y);
    this->part2.setPosition(x, y + this->HEIGHT - 2);
    this->part3.setPosition(x, y + (this->HEIGHT - 2) * 2);
    this->part4.setPosition(x, y + (this->HEIGHT - 2) * 3);

    // Se establece la rotación de cada parte del rayo para formar una figura de relámpago.
    this->part1.setRotation(45);
    this->part2.setRotation(315);
    this->part3.setRotation(45);
    this->part4.setRotation(315);
}

// La función "flip" hace que el relámpago gire en la dirección en que mira.
// Disminuye el contador de rotación "flip_tick" y, cuando alcanza cero, rota cada parte del rayo 90 grados.
void PoweredInvaderLaser::flip()
{
    --this->flip_tick;
    if (this->flip_tick <= 0)
    {
        this->part1.rotate(90);
        this->part2.rotate(90);
        this->part3.rotate(90);
        this->part4.rotate(90);

        this->flip_tick = this->FLIP_TICK_MAX;
    }
}

// La función "getX" devuelve la posición X de la segunda parte del rayo.
unsigned PoweredInvaderLaser::getX() const
{
    return this->part2.getPosition().x;
}

// La función "getY" devuelve la posición Y de la segunda parte del rayo.
unsigned PoweredInvaderLaser::getY() const
{
    return this->part2.getPosition().y;
}

// La función "move" mueve cada parte del rayo hacia abajo para simular el movimiento del láser motorizado.
// También llama a la función "flip" para que el rayo gire durante el movimiento.
// Cuando el rayo cruza la línea de los escudos, cambia el color a verde.
void PoweredInvaderLaser::move()
{
    this->flip();
    this->part1.move(0, this->SPEED);
    this->part2.move(0, this->SPEED);
    this->part3.move(0, this->SPEED);
    this->part4.move(0, this->SPEED);

    // Cambia el color a verde una vez que el rayo cruza la línea de los escudos.
    if (this->part4.getPosition().y >= this->SHIELD_LINE)
    {
        this->part1.setFillColor(sf::Color::Green);
        this->part2.setFillColor(sf::Color::Green);
        this->part3.setFillColor(sf::Color::Green);
        this->part4.setFillColor(sf::Color::Green);
    }
}

// La función "draw" dibuja cada parte del rayo en la ventana proporcionada.
void PoweredInvaderLaser::draw(sf::RenderWindow& window)
{
    window.draw(this->part1);
    window.draw(this->part2);
    window.draw(this->part3);
    window.draw(this->part4);
}

// La función "checkCollide" verifica si el punto (x, y) colisiona con alguna parte del rayo.
// Devuelve verdadero si hay colisión, falso en caso contrario.
bool PoweredInvaderLaser::checkCollide(const unsigned x, const unsigned y) const
{
    if (this->part1.getGlobalBounds().contains(x, y) || this->part2.getGlobalBounds().contains(x, y) || this->part3.getGlobalBounds().contains(x, y) || this->part4.getGlobalBounds().contains(x, y))
        return true;

    return false;
}

// La función "checkCollide" verifica si un rectángulo colisiona con alguna parte del rayo.
// Devuelve verdadero si hay colisión, falso en caso contrario.
bool PoweredInvaderLaser::checkCollide(const sf::FloatRect rect) const
{
    if (rect.intersects(this->part1.getGlobalBounds()) || rect.intersects(this->part2.getGlobalBounds()) || rect.intersects(this->part3.getGlobalBounds()) || rect.intersects(this->part4.getGlobalBounds()))
        return true;

    return false;
}
