#pragma once
#ifndef INVADERLASER_H
#define INVADERLASER_H

#include <SFML/Graphics.hpp>
#include "globals.h"
#include "Invader.h"

/* El disparo l�ser de Space Invaders. */
class InvaderLaser
{
    // El l�ser motorizado parece un rayo y hace m�s da�o a los escudos
public: enum LaserType { NORMAL, POWERED };

protected:
    const LaserType TYPE;
    const int SPEED; // Qu� tan r�pido se mueve el l�ser.
    const unsigned SHIELD_DMG; // Cuanto da�o hace a los escudos
    const unsigned SHIELD_LINE;

    bool hit; // Si golpea o no algo ya punto de ser eliminado.
    bool will_hurt; // Si el l�ser da�ar� o no al jugador.

    // El invasor que dispar� esta bala.
    Invader& owner;
    // Hacer que el constructor est� protegido porque InvaderLasers nunca debe instanciarse directamente.
    InvaderLaser(const LaserType type, const int speed, const unsigned shield_dmg, const unsigned shield_line, const bool will_hurt, Invader& owner) : TYPE(type), SPEED(speed), SHIELD_DMG(shield_dmg), SHIELD_LINE(shield_line), hit(false), will_hurt(will_hurt), owner(owner)
    {
        owner.incLasersOnScreen();
    }

public:

    virtual ~InvaderLaser()
    {
        this->owner.decLasersOnScreen();
    }

    /* M�todos cuyo comportamiento es el mismo para ambos tipos de l�seres. */
    bool isHit() const { return this->hit; }
    bool willHurt() const { return this->will_hurt; }
    void setHit() { this->hit = true; }
    unsigned getDmg() const { return this->SHIELD_DMG; }
    LaserType getType() const { return this->TYPE; }
    void remove() { this->hit = true; }

    /* Los siguientes son todos m�todos virtuales puros y DEBEN ser implementados por subclases.
      * Su comportamiento difiere seg�n el tipo de l�ser. */
    virtual unsigned getX() const = 0;
    virtual unsigned getY() const = 0;
    virtual void move() = 0;
    virtual void draw(sf::RenderWindow& window) = 0;

    // Sobrecargar el m�todo checkCollide para permitir la verificaci�n contra rect o point.
    virtual bool checkCollide(const sf::FloatRect rect) const = 0;
    virtual bool checkCollide(const unsigned x, const unsigned y) const = 0;

};

#endif


