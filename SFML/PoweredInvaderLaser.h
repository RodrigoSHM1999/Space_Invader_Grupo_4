#pragma once
#ifndef POWEREDINVADERLASER_H
#define POWEREDINVADERLASER_H

#include "InvaderLaser.h"

/* Un láser motorizado se mueve más rápido y hace más daño a los escudos. */

class PoweredInvaderLaser : public InvaderLaser
{
private:
    static constexpr unsigned WIDTH = 2; // Ancho del láser motorizado
    static constexpr unsigned HEIGHT = 8; // Alto del láser motorizado

    static constexpr unsigned FLIP_TICK_MAX = 7; // Número máximo de "ticks" para cambiar la dirección del láser

    sf::RectangleShape part1; // Primera parte del rayo láser
    sf::RectangleShape part2; // Segunda parte del rayo láser
    sf::RectangleShape part3; // Tercera parte del rayo láser
    sf::RectangleShape part4; // Cuarta parte del rayo láser

    unsigned flip_tick; // Contador para controlar el tiempo de cambio de dirección del láser

    // Función privada para cambiar la dirección del rayo láser
    void flip();

public:
    // Constructor del láser motorizado, recibe las coordenadas (x, y) de inicio, la línea del escudo,
    // un indicador si el láser hará daño a los escudos y una referencia al invader que lo dispara.
    PoweredInvaderLaser(const unsigned x, const unsigned y,
        const unsigned shield_line, const bool will_hurt,
        Invader& owner);

    // Destructor virtual (clase base InvaderLaser es una clase abstracta)
    virtual ~PoweredInvaderLaser() {}

    // Métodos virtuales heredados de InvaderLaser

    // Devuelve la coordenada X de la posición del láser
    virtual unsigned getX() const;

    // Devuelve la coordenada Y de la posición del láser
    virtual unsigned getY() const;

    // Mueve el láser en su dirección actual
    virtual void move();

    // Dibuja el láser en la ventana proporcionada
    virtual void draw(sf::RenderWindow& window);

    // Sobrecargar el método checkCollide para permitir colisiones con puntos (x, y) o rectángulos
    virtual bool checkCollide(const unsigned x, const unsigned y) const;
    virtual bool checkCollide(const sf::FloatRect rect) const;

};

#endif
