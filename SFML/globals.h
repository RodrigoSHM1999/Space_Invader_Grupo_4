#pragma once
#ifndef GLOBALS_H
#define GLOBALS_H

#include <string>
#include <SFML/Graphics.hpp>

/* Globales útiles para todas las clases. */


namespace Globals
{
    // Los diferentes estados en los que puede estar el juego.
    enum States
    {
        MENU, // Menú de inicio del juego
        PLAY, // El jugador está jugando actualmente el juego
        PAUSED, // El jugador ha pausado el juego
        WAVE_SETUP, // Cuando comienza una nueva oleada, esto muestra a los Invaders de manera especial
        PLAYER_KILLED, // La nave del jugador acaba de ser alcanzada y está reproduciendo la animación de muerte
        GAME_OVER // Se acabaron todas las vidas, o los Invaders han llegado a la parte inferior de la pantalla.
    };

    const std::string SCREEN_TITLE = "Space Invaders!";
    constexpr unsigned SCREEN_WIDTH = 800;
    constexpr unsigned SCREEN_HEIGHT = 730;
    constexpr unsigned FRAME_RATE = 60;
    const sf::Color BG_COLOR = sf::Color::Black;

    const std::string SOUNDS_PATH = "sounds/";
    const std::string SPRITES_PATH = "sprites/";
    const std::string FONTS_PATH = "fonts/";

    const std::string HS_FILE = "misc/definitely_NOT_highscore.sav";

    extern States GAME_STATE;
}

#endif

