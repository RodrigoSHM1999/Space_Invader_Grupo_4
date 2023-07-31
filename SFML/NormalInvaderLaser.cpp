#include "NormalInvaderLaser.h"

// M�todo privado para animar el movimiento ascendente y descendente de la parte horizontal del l�ser.
void NormalInvaderLaser::moveHPart()
{
    // Obtener las posiciones actuales de las partes horizontal y vertical del l�ser.
    unsigned h_part_y = this->h_part.getPosition().y;
    unsigned v_part_y = this->v_part.getPosition().y;

    // Cambiar la direcci�n de movimiento de la parte horizontal si alcanza los l�mites superior o inferior.
    if ((h_part_y >= (v_part_y + this->HEIGHT - this->THICKNESS)) && this->h_part_movedir == 1)
        this->h_part_movedir = -1;
    else if ((h_part_y <= (v_part_y)) && this->h_part_movedir == -1)
        this->h_part_movedir = 1;

    // Si se est� moviendo hacia abajo, es necesario moverse ligeramente m�s r�pido que la parte vertical.
    // Permanece inm�vil si se est� moviendo hacia arriba (ilusi�n creada por la parte vertical que sigue bajando).
    if (this->h_part_movedir == 1)
        this->h_part.move(0, this->SPEED * this->h_part_moverate);
}

// Constructor que inicializa las partes del l�ser normal y establece su posici�n inicial.
NormalInvaderLaser::NormalInvaderLaser(const unsigned x, const unsigned y, const unsigned shield_line, const bool will_hurt, Invader& owner) : InvaderLaser(LaserType::NORMAL, 5, 15, shield_line, will_hurt, owner), h_part_movedir(1)
{
    // Crear las formas que componen el l�ser normal.
    this->v_part = sf::RectangleShape(sf::Vector2f(this->THICKNESS, this->HEIGHT));
    this->h_part = sf::RectangleShape(sf::Vector2f(this->WIDTH, this->THICKNESS));

    // Establecer el color de las partes del l�ser a blanco.
    this->v_part.setFillColor(sf::Color::White);
    this->h_part.setFillColor(sf::Color::White);

    // Establecer la posici�n inicial de las partes del l�ser.
    this->v_part.setPosition(x + ((this->WIDTH / 2) - 1), y);
    this->h_part.setPosition(x, y);
}

// M�todo para obtener la posici�n en el eje X del l�ser.
unsigned NormalInvaderLaser::getX() const
{
    return this->v_part.getPosition().x;
}

// M�todo para obtener la posici�n en el eje Y del l�ser.
unsigned NormalInvaderLaser::getY() const
{
    return this->v_part.getPosition().y;
}

// M�todo para mover el l�ser hacia abajo (posici�n inferior de la pantalla) y animar la parte horizontal.
void NormalInvaderLaser::move()
{
    // Mover las partes vertical y horizontal del l�ser hacia abajo.
    this->v_part.move(0, this->SPEED);
    this->moveHPart();

    // Cambiar el color a verde una vez que el l�ser pasa los escudos.
    if (this->v_part.getPosition().y >= this->SHIELD_LINE)
    {
        this->v_part.setFillColor(sf::Color::Green);
        this->h_part.setFillColor(sf::Color::Green);
    }
}

// M�todo para dibujar las partes del l�ser en la ventana proporcionada.
void NormalInvaderLaser::draw(sf::RenderWindow& window)
{
    window.draw(this->v_part);
    window.draw(this->h_part);
}

// M�todo para verificar si el l�ser colisiona con una posici�n espec�fica (indicada por las coordenadas x e y).
bool NormalInvaderLaser::checkCollide(const unsigned x, const unsigned y) const
{
    // Verificar si las partes vertical u horizontal del l�ser contienen las coordenadas x e y proporcionadas.
    if (this->v_part.getGlobalBounds().contains(x, y) || this->h_part.getGlobalBounds().contains(x, y))
        return true;

    return false;
}

// M�todo para verificar si el l�ser colisiona con un rect�ngulo espec�fico, que se proporciona como argumento.
bool NormalInvaderLaser::checkCollide(const sf::FloatRect rect) const
{
    // Verificar si las partes vertical u horizontal del l�ser intersectan con el rect�ngulo proporcionado.
    if (rect.intersects(this->v_part.getGlobalBounds()) || rect.intersects(this->h_part.getGlobalBounds()))
        return true;

    return false;
}
