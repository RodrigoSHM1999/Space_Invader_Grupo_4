#pragma once
#ifndef INVADERLASER_H
#define INVADERLASER_H

#include <SFML/Graphics.hpp>
#include "globals.h"
#include "Invader.h"

/* El disparo láser de Space Invaders. */
class InvaderLaser
{
    // El láser motorizado parece un rayo y hace más daño a los escudos
public: enum LaserType { NORMAL, POWERED };

protected:
    const LaserType TYPE;
    const int SPEED; // Qué tan rápido se mueve el láser.
    const unsigned SHIELD_DMG; // Cuanto daño hace a los escudos
    const unsigned SHIELD_LINE;

    bool hit; // Si golpea o no algo ya punto de ser eliminado.
    bool will_hurt; // Si el láser dañará o no al jugador.

    // El invasor que disparó esta bala.
    Invader& owner;
    // Hacer que el constructor esté protegido porque InvaderLasers nunca debe instanciarse directamente.
    InvaderLaser(const LaserType type, const int speed, const unsigned shield_dmg, const unsigned shield_line, const bool will_hurt, Invader& owner) : TYPE(type), SPEED(speed), SHIELD_DMG(shield_dmg), SHIELD_LINE(shield_line), hit(false), will_hurt(will_hurt), owner(owner)
    {
        owner.incLasersOnScreen();
    }

public:

    virtual ~InvaderLaser()
    {
        this->owner.decLasersOnScreen();
    }

    /* Métodos cuyo comportamiento es el mismo para ambos tipos de láseres. */
    bool isHit() const { return this->hit; }
    bool willHurt() const { return this->will_hurt; }
    void setHit() { this->hit = true; }
    unsigned getDmg() const { return this->SHIELD_DMG; }
    LaserType getType() const { return this->TYPE; }
    void remove() { this->hit = true; }

    /* Los siguientes son todos métodos virtuales puros y DEBEN ser implementados por subclases.
      * Su comportamiento difiere según el tipo de láser. */
    virtual unsigned getX() const = 0;
    virtual unsigned getY() const = 0;
    virtual void move() = 0;
    virtual void draw(sf::RenderWindow& window) = 0;

    // Sobrecargar el método checkCollide para permitir la verificación contra rect o point.
    virtual bool checkCollide(const sf::FloatRect rect) const = 0;
    virtual bool checkCollide(const unsigned x, const unsigned y) const = 0;

};

#endif


