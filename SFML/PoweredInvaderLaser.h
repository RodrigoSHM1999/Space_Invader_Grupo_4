#pragma once
#ifndef POWEREDINVADERLASER_H
#define POWEREDINVADERLASER_H

#include "InvaderLaser.h"

/* Un l�ser motorizado se mueve m�s r�pido y hace m�s da�o a los escudos. */

class PoweredInvaderLaser : public InvaderLaser
{
private:
    static constexpr unsigned WIDTH = 2; // Ancho del l�ser motorizado
    static constexpr unsigned HEIGHT = 8; // Alto del l�ser motorizado

    static constexpr unsigned FLIP_TICK_MAX = 7; // N�mero m�ximo de "ticks" para cambiar la direcci�n del l�ser

    sf::RectangleShape part1; // Primera parte del rayo l�ser
    sf::RectangleShape part2; // Segunda parte del rayo l�ser
    sf::RectangleShape part3; // Tercera parte del rayo l�ser
    sf::RectangleShape part4; // Cuarta parte del rayo l�ser

    unsigned flip_tick; // Contador para controlar el tiempo de cambio de direcci�n del l�ser

    // Funci�n privada para cambiar la direcci�n del rayo l�ser
    void flip();

public:
    // Constructor del l�ser motorizado, recibe las coordenadas (x, y) de inicio, la l�nea del escudo,
    // un indicador si el l�ser har� da�o a los escudos y una referencia al invader que lo dispara.
    PoweredInvaderLaser(const unsigned x, const unsigned y,
        const unsigned shield_line, const bool will_hurt,
        Invader& owner);

    // Destructor virtual (clase base InvaderLaser es una clase abstracta)
    virtual ~PoweredInvaderLaser() {}

    // M�todos virtuales heredados de InvaderLaser

    // Devuelve la coordenada X de la posici�n del l�ser
    virtual unsigned getX() const;

    // Devuelve la coordenada Y de la posici�n del l�ser
    virtual unsigned getY() const;

    // Mueve el l�ser en su direcci�n actual
    virtual void move();

    // Dibuja el l�ser en la ventana proporcionada
    virtual void draw(sf::RenderWindow& window);

    // Sobrecargar el m�todo checkCollide para permitir colisiones con puntos (x, y) o rect�ngulos
    virtual bool checkCollide(const unsigned x, const unsigned y) const;
    virtual bool checkCollide(const sf::FloatRect rect) const;

};

#endif
