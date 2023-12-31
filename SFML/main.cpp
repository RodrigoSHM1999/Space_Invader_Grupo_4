#include <ctime>
#include <SFML/Graphics.hpp>
#include "globals.h"
#include "misc.h"
#include "game.h"
#include "SoundFx.h"
#include "Textures.h"

int main()
{
    unsigned game_score = 0;
    unsigned wave_on = 0;

    srand(time(0));

    /*sf::Clock fps_clock;
    float fps_timer = fps_clock.getElapsedTime().asSeconds();*/

    sf::RenderWindow window(sf::VideoMode(Globals::SCREEN_WIDTH, Globals::SCREEN_HEIGHT), Globals::SCREEN_TITLE);
    window.setFramerateLimit(Globals::FRAME_RATE);

    /*sf::Image icon;
    icon.loadFromFile("images/icon.png");
    window.setIcon(32, 32, icon.getPixelsPtr());*/

    // This loads all textures.
    Textures textures;

    SoundFx shoot_snd(Globals::SOUNDS_PATH + "shoot_laser.wav");
    shoot_snd.setVolume(70);

    SoundFx player_killed_snd(Globals::SOUNDS_PATH + "player_death.wav");

    SoundFx ufo_move_snd(Globals::SOUNDS_PATH + "ufo_move.wav");
    SoundFx ufo_killed_snd(Globals::SOUNDS_PATH + "ufo_killed.wav");
    ufo_move_snd.setVolume(50);

    SoundFx invader_death_snd(Globals::SOUNDS_PATH + "invader_death.wav");
    invader_death_snd.setVolume(70);

    SoundFx invader_step1_snd(Globals::SOUNDS_PATH + "invader_step1.wav");
    SoundFx invader_step2_snd(Globals::SOUNDS_PATH + "invader_step2.wav");
    SoundFx invader_step3_snd(Globals::SOUNDS_PATH + "invader_step3.wav");
    SoundFx invader_step4_snd(Globals::SOUNDS_PATH + "invader_step4.wav");

    Earth earth(Globals::SCREEN_WIDTH);

    UFO ufo(textures, ufo_move_snd, ufo_killed_snd);

    InvaderFormation invaders(window, textures, earth, invader_step1_snd, invader_step2_snd, invader_step3_snd, invader_step4_snd, invader_death_snd);

    ShieldWall shields(window, textures, Globals::SCREEN_WIDTH);
    invaders.setShieldLine(shields.getY() - 20);

    CoreCannon cannon(textures, player_killed_snd, 100);

    PlayerLaser player_laser(shoot_snd);

    ScoreDisplay score_disp(game_score);

    LivesDisplay lives_disp(textures);

    Menu menu(textures, score_disp);

    Explosions explosions(textures);


    /* Begin game loop */
    while (window.isOpen())
    {
        Game::handle_events(window, menu, score_disp, lives_disp, invaders, player_laser, cannon, shields, earth, ufo, explosions, wave_on);
        Game::update_objects(cannon, player_laser, earth, ufo, invaders, shields, lives_disp, explosions, game_score, wave_on);
        Game::draw_objects(window, menu, score_disp, lives_disp, ufo, invaders, shields, cannon, player_laser, earth, explosions, wave_on);
        //Game::updateFPS(window, fps_clock, fps_timer);
    }
}
