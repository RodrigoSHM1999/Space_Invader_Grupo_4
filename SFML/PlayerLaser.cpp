#include <cmath>
#include "PlayerLaser.h"
#include "UFO.h"

PlayerLaser::PlayerLaser(SoundFx& shoot_snd) : is_shooting(false), shoot_snd(shoot_snd)
{
    // Crea el l�ser del jugador (solo puede haber 1 l�ser del jugador en pantalla a la vez)
    this->laser = sf::RectangleShape(sf::Vector2f(this->WIDTH, this->HEIGHT));
    this->laser.setPosition(999, 999); // Posici�n fuera de la pantalla hasta que se dispare
    this->laser.setFillColor(sf::Color::White);
    this->laser.setOrigin(round(this->WIDTH / 2), this->HEIGHT); // Hace que el origen del l�ser est� en la parte inferior

}

void PlayerLaser::shoot(const unsigned startx, const unsigned starty, UFO& ufo)
{
    // Si el l�ser ya est� disparando, no intentar disparar nuevamente
    if (!this->is_shooting)
    {
        this->shoot_snd.play(); // Reproduce el efecto de sonido de disparo
        this->laser.setPosition(startx, starty); // Coloca el l�ser en la posici�n de inicio
        ufo.incShotsFired(); // Incrementa la cantidad de disparos de UFO
        this->is_shooting = true; // Establece el estado de disparo a verdadero
    }
}

void PlayerLaser::move(Explosions& explosions)
{
    // Probablemente se llame a esto en cada cuadro, as� que no mover a menos que est� disparando
    if (!this->is_shooting)
        return;

    this->laser.move(0, this->SPEED * -1); // Mueve el l�ser hacia arriba seg�n la velocidad

    // El jugador puede disparar nuevamente cuando el l�ser sale de la pantalla
    if (this->laser.getPosition().y < 100)
    {
        explosions.newExplosion(sf::Color::Red, this->getShape().getPosition().x, this->getShape().getPosition().y); // Crea una nueva explosi�n en la posici�n del l�ser
        this->stop(); // Detiene el disparo del l�ser
    }
}

void PlayerLaser::stop()
{
    this->is_shooting = false; // Establece el estado de disparo a falso

    // Hace esto para que el l�ser no est� esperando una colisi�n.
    this->laser.setPosition(999, 999); // Establece la posici�n del l�ser fuera de la pantalla.
}
