#pragma once
#ifndef SHIELD_H
#define SHIELD_H

#include <SFML/Graphics.hpp>
#include "Textures.h"
#include "PlayerLaser.h"
#include "InvaderFormation.h"

/* Los escudos protegen al jugador del fuego entrante. */

class Shield
{
private:
    // Los escudos individuales tienen una textura única porque se pueden destruir.
    sf::Texture texture; // Textura del escudo
    sf::Sprite sprite; // Sprite que representa visualmente el escudo

    // Imagen hecha de textura para colisión de píxeles
    sf::Image img; // Imagen correspondiente a la textura del escudo para colisiones de píxeles
    Textures& textures; // Referencia a las texturas necesarias para el escudo

public:
    // Constructor del escudo, recibe referencias a las texturas y las coordenadas (x, y) de inicio
    Shield(Textures& textures, const int xpos, const int ypos);

    // Devuelve una referencia al sprite del escudo
    sf::Sprite& getSprite() { return this->sprite; }

    // Restablece el escudo a su estado inicial
    void reset();

    // Maneja las colisiones con los láseres de los jugadores e invaders
    void handleCollide(PlayerLaser& player_laser, Lasers& invader_lasers, Explosions& explosions);

    // Los invaders destruyen el escudo al pasar sobre él
    void handleCollideInvaders(InvaderFormation& invaders);

    // Agrega daño al escudo en la posición (x, y) con un valor específico de daño (dmg)
    void damageShield(int x, int y, const int dmg);

};

#endif
