#pragma once
#ifndef TEXTURES_H
#define TEXTURES_H

#include <SFML/Graphics.hpp>

/* Esta clase carga todas las texturas para que las usen los sprites. */

class Textures
{
private:
    sf::Image spritesheet;

    sf::Image loadSprites(const std::string img);

public:
    sf::Texture UFO;
    sf::Texture UFO_MENU;

    sf::Texture LARGE_INV_1;
    sf::Texture LARGE_INV_2;

    sf::Texture MEDIUM_INV_1;
    sf::Texture MEDIUM_INV_2;

    sf::Texture SMALL_INV_1;
    sf::Texture SMALL_INV_2;

    sf::Texture INVADER_DEATH;

    sf::Texture SHIELD;

    sf::Texture CORECANNON;
    sf::Texture CORECANNON_DEATH_1;
    sf::Texture CORECANNON_DEATH_2;

    sf::Texture EXPLOSION;

    // Carga las texturas en la memoria para que las usen los sprites.
    Textures();

};

#endif

