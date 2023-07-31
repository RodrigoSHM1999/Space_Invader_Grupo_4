#pragma once
#ifndef INVADER_H
#define INVADER_H

#include <SFML/Graphics.hpp>
#include "Textures.h"

/* Un invasor espacial individual. */

class Invader
{
    // Esto está aquí arriba porque me gustan los miembros privados primero,
    // pero esto debe ser público y es utilizado por un miembro privado.
    // Los diferentes tipos de invasores
public: enum InvaderType { LARGE, MEDIUM, SMALL };

private:
    static constexpr unsigned HEIGHT = 35; // Altura del invasor
    static constexpr unsigned DEATH_TICK_MAX = 10; // Cuánto tiempo debe mostrarse el sprite de muerte
    static constexpr int SPEED = 10; // Cuánto se mueve el invasor en cada paso

    // Los invasores tienen 2 fotogramas en su animación.
    // Son punteros en lugar de referencias porque no se pueden inicializar por defecto.
    sf::Texture* frame1_txtr;
    sf::Texture* frame2_txtr;
    sf::Texture* frame3_txtr;
    sf::Sprite sprite; // Sprite del invasor

    InvaderType type; // Tipo del invasor
    unsigned score_value; // Cuántos puntos vale el invasor cuando es destruido
    int move_dir; // 1 significa derecha, -1 significa izquierda, 0 significa que no se está moviendo
    bool is_dead; // Indica si el invasor está muerto
    bool is_exploding; // Indica si el invasor está explotando (muriendo)
    int frame_on; // Puede ser 1 o 2, indica el fotograma actual de la animación

    unsigned death_tick; // Lleva el registro de cuánto tiempo ha mostrado el sprite de muerte

    unsigned lasers_on_screen; // Cantidad de láseres del invasor actualmente en pantalla

    // Cambia al siguiente fotograma de la animación
    void flipFrame();

public:
    Invader(Textures& textures, const InvaderType type); // Constructor que recibe una referencia a las texturas y el tipo de invasor
    sf::Sprite& getSprite() { return this->sprite; } // Obtiene el sprite del invasor
    static unsigned getHeight() { return Invader::HEIGHT; } // Obtiene la altura del invasor
    unsigned getScoreValue() const { return this->score_value; } // Obtiene el valor de puntos del invasor
    bool isDead() const { return this->is_dead; } // Comprueba si el invasor está muerto
    bool isExploding() const { return this->is_exploding; } // Comprueba si el invasor está explotando (muriendo)
    int getMoveDir() const { return this->move_dir; } // Obtiene la dirección actual de movimiento del invasor
    unsigned getLasersOnScreen() const { return this->lasers_on_screen; } // Obtiene la cantidad de láseres del invasor en pantalla

    void reset(const unsigned x, const unsigned y); // Reinicia el invasor en las coordenadas dadas

    // Llamado cuando el invasor es impactado por una bala
    void die();

    // Llamado en cada tick de movimiento
    void move();

    // El invasor desciende una fila
    void dropDown(const unsigned shield_line);

    // El invasor invierte su dirección de movimiento
    void reverseDir();

    // Comprueba si el invasor ha alcanzado el borde de la pantalla
    bool checkHitEdge(const int screenw);

    // Incrementa el contador de tick de muerte y oculta el invasor una vez que alcanza el máximo
    void incDeathTick();

    void incLasersOnScreen() { ++this->lasers_on_screen; } // Incrementa el número de láseres del invasor en pantalla
    void decLasersOnScreen() { --this->lasers_on_screen; } // Decrementa el número de láseres del invasor en pantalla

};

#endif


