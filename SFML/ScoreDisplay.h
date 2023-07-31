#pragma once
#ifndef SCOREDISPLAY_H
#define SCOREDISPLAY_H

#include <SFML/Graphics.hpp>

class ScoreDisplay
{
private:
    static constexpr unsigned X = 0;
    static constexpr unsigned Y = 0;

    // Referencia a la puntuación del juego, que es una variable declarada en main().
    unsigned& game_score;

    unsigned highscore;

public:
    ScoreDisplay(unsigned& score);

    void drawScore(sf::RenderWindow& window, const unsigned score, const unsigned x, const unsigned y);
    void draw(sf::RenderWindow& window);
    void reset();

};

#endif

