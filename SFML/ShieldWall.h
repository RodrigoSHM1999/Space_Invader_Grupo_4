#pragma once
#ifndef SHIELDWALL_H
#define SHIELDWALL_H

#include <SFML/Graphics.hpp>
#include "Shield.h"

/* Controla el comportamiento de cada escudo en pantalla.
      * Principalmente solo verifica colisiones con láseres. */

typedef std::vector<Shield*> ShieldVector;

class ShieldWall
{
private:
    static constexpr unsigned Y = 520;

    // Dónde comienza el primer escudo y la brecha entre los escudos subsiguientes
    static constexpr unsigned STARTX = 97;
    static constexpr unsigned GAP = 175;

    ShieldVector shields;

    sf::RenderWindow& window;

public:
    ShieldWall(sf::RenderWindow& window, Textures& textures, const unsigned screenw);
    ~ShieldWall();

    void reset();

    unsigned getY() const { return this->Y; }

    void handleCollisions(PlayerLaser& laser, Lasers& invader_lasers, InvaderFormation& invaders, Explosions& explosions);

    void draw();

};

#endif

