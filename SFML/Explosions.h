#pragma once
#ifndef EXPLOSIONS_H
#define EXPLOSIONS_H

#include <vector>
#include <SFML/Graphics.hpp>
#include "Textures.h"

/* Esta clase representa una sola explosión en el juego. */
class Explosion
{
private:
    static constexpr unsigned SHOW_TICK_START = 20;

    sf::Sprite sprite;
    unsigned show_tick;
    bool showing;

public:
    /* Constructor de la clase. */
    Explosion(Textures& textures, sf::Color color, const unsigned x, const unsigned y);

    /* Obtiene el estado actual de la explosión. */
    bool isShowing() const { return this->showing; }

    /* Actualiza el estado de la explosión. */
    void update();

    /* Dibuja la explosión en una ventana SFML. */
    void draw(sf::RenderWindow& window);

};



/* Esta clase es un contenedor que maneja múltiples explosiones. */
class Explosions
{
private:
    std::vector<Explosion*> explosions;
    Textures& textures;

public:
    /* Constructor de la clase. */
    Explosions(Textures& textures);

    /* Destructor de la clase. */
    ~Explosions();

    /* Reinicia el estado del sistema de explosiones. */
    void reset();

    /* Actualiza el estado de todas las explosiones. */
    void update();

    /* Dibuja todas las explosiones en la ventana SFML. */
    void draw(sf::RenderWindow& window);

    /* Crea una nueva explosión en las coordenadas especificadas. */
    void newExplosion(sf::Color color, const unsigned x, const unsigned y);

};
#endif

