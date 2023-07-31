#include <sstream>
#include "ScoreDisplay.h"
#include "game.h"

ScoreDisplay::ScoreDisplay(unsigned& score) : game_score(score), highscore(0)
{
    this->reset();
}

void ScoreDisplay::reset()
{
    if (this->game_score > this->highscore)
        Game::write_highscore(this->game_score);

    this->game_score = 0;
    this->highscore = Game::get_highscore();
}

void ScoreDisplay::drawScore(sf::RenderWindow& window, const unsigned score, const unsigned x, const unsigned y)
{
    // Dibuja cada d�gito de la puntuaci�n, con un 0 inicial si es necesario.
    std::ostringstream ss_score;
    ss_score << (score / 1000) % 10 << " " << (score / 100) % 10 << " " << (score / 10) % 10 << " " << (score / 1) % 10;
    Game::draw_text(window, ss_score.str(), x, y);
}

void ScoreDisplay::draw(sf::RenderWindow& window)
{
    std::ostringstream scoreboard;
    scoreboard << "P U N T O S < 1 >      M A X . P U N T O S     P U N T O S < 2 >  ";
    Game::draw_text(window, scoreboard.str(), this->X + 80, this->Y + 20);

    this->drawScore(window, this->game_score, this->X + 140, this->Y + 60);

    // Dibuja la puntuaci�n alta.
    this->drawScore(window, this->highscore, this->X + 330, this->Y + 60);

    if (Globals::GAME_STATE == Globals::States::MENU)
        Game::draw_text(window, "0 0 0 0", this->X + 575, this->Y + 60);
}