#pragma once
#ifndef PLAYERLASER_H
#define PLAYERLASER_H

#include <SFML/Graphics.hpp>
#include "SoundFx.h"
#include "Explosions.h"

class UFO;

/* El l�ser disparado por el jugador.
 * Solo puede haber 1 en pantalla a la vez. */

class PlayerLaser
{
private:
    static constexpr unsigned WIDTH = 3; // Ancho del l�ser
    static constexpr unsigned HEIGHT = 15; // Alto del l�ser
    static constexpr unsigned SHIELD_DMG = 10; // Da�o que inflige al escudo del enemigo

    static constexpr int SPEED = 10; // Velocidad de movimiento del l�ser
    sf::RectangleShape laser; // Forma del l�ser
    bool is_shooting; // Indica si el l�ser est� disparando

    SoundFx& shoot_snd; // Referencia al efecto de sonido de disparo

public:
    PlayerLaser(SoundFx& shoot_snd); // Constructor que recibe el efecto de sonido de disparo
    sf::RectangleShape& getShape() { return this->laser; } // Obtiene la forma del l�ser
    bool isShooting() const { return this->is_shooting; } // Comprueba si el l�ser est� disparando actualmente
    unsigned getDmg() const { return this->SHIELD_DMG; } // Obtiene el da�o que inflige el l�ser al escudo del enemigo

    // Inicia el movimiento de disparo
    void shoot(const unsigned startx, const unsigned starty, UFO& ufo);

    // El l�ser se mueve hacia arriba despu�s del disparo
    void move(Explosions& explosions);

    // Llamado para detener el disparo del l�ser
    void stop();

};

#endif
