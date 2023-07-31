#pragma once
#ifndef PLAYERLASER_H
#define PLAYERLASER_H

#include <SFML/Graphics.hpp>
#include "SoundFx.h"
#include "Explosions.h"

class UFO;

/* El láser disparado por el jugador.
 * Solo puede haber 1 en pantalla a la vez. */

class PlayerLaser
{
private:
    static constexpr unsigned WIDTH = 3; // Ancho del láser
    static constexpr unsigned HEIGHT = 15; // Alto del láser
    static constexpr unsigned SHIELD_DMG = 10; // Daño que inflige al escudo del enemigo

    static constexpr int SPEED = 10; // Velocidad de movimiento del láser
    sf::RectangleShape laser; // Forma del láser
    bool is_shooting; // Indica si el láser está disparando

    SoundFx& shoot_snd; // Referencia al efecto de sonido de disparo

public:
    PlayerLaser(SoundFx& shoot_snd); // Constructor que recibe el efecto de sonido de disparo
    sf::RectangleShape& getShape() { return this->laser; } // Obtiene la forma del láser
    bool isShooting() const { return this->is_shooting; } // Comprueba si el láser está disparando actualmente
    unsigned getDmg() const { return this->SHIELD_DMG; } // Obtiene el daño que inflige el láser al escudo del enemigo

    // Inicia el movimiento de disparo
    void shoot(const unsigned startx, const unsigned starty, UFO& ufo);

    // El láser se mueve hacia arriba después del disparo
    void move(Explosions& explosions);

    // Llamado para detener el disparo del láser
    void stop();

};

#endif
