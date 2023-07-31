#pragma once
#ifndef INVADERFORMATION_H
#define INVADERFORMATION_H

#include <SFML/Graphics.hpp>
#include "SoundFx.h"
#include "Invader.h"
#include "NormalInvaderLaser.h"
#include "PoweredInvaderLaser.h"
#include "PlayerLaser.h"
#include "Earth.h"
#include "LivesDisplay.h"
#include "Explosions.h"
#include "PlayerLaser.h"

class CoreCannon;

/* Representa toda la formación de Space Invader.
 * Coordina el movimiento y disparo de todos los Space Invaders. */

typedef std::vector<Invader*> InvaderRow;
typedef std::vector<InvaderRow> InvaderVector2D;
typedef std::vector<InvaderLaser*> Lasers;

class InvaderFormation
{
private:
    // Tamaño de la formación
    static constexpr unsigned COLUMNS = 5;
    static constexpr unsigned ROWS = 11;

    // En donde comienza la formación en la pantalla
    static constexpr unsigned STARTX = 120;
    const unsigned STARTY = 150;

    // El espacio entre cada invasor en la formación
    static constexpr unsigned XGAP = 55;
    static constexpr unsigned YGAP = 50;

    static constexpr unsigned WAVE_SHIFT_Y = 50;

    static constexpr unsigned INVADE_LINE = Globals::SCREEN_HEIGHT - 110;

    // Rango de movimientos dentro del juego
    static constexpr int MOVE_TICK_MAX_START = 45;
    static constexpr int MOVE_TICK_CHANGE_START = 1;//5;

    // Probabilidad de que cada Invasor dispare: 1/SHOT_CHANCE
    static constexpr int SHOT_CHANCE_START = 1000;

    unsigned SHIELD_LINE;

    sf::RenderWindow& window;
    InvaderVector2D invaders;

    // Tierra, la línea en la parte inferior de la pantalla.
    Earth& earth;

    // Cuando move_tick golpea move_tick_max, la formación se mueve
    // move_tick_max se reduce cada vez que la formación toca el borde de la pantalla
    int move_tick;
    int move_tick_max;
    int move_tick_change;

    unsigned step_on;
    unsigned num_killed; // Número de invasores asesinados.

    bool has_hit_edge;

    int shot_chance;

    // Sonidos utilizados
    SoundFx& step1;
    SoundFx& step2;
    SoundFx& step3;
    SoundFx& step4;
    SoundFx& death_snd;


    // Verifica si los invasores están explotando y aumenta la marca.
    void incDeathTick();

    // Ciclos a través de 4 sonidos diferentes en cada paso.
    void playStepSound();

    // Aumenta la velocidad de disparo al reducir la probabilidad de disparo.
    void increaseFire(const int amount);

    // Métodos privados que se ocupan de los láseres.
    bool toShootOrNah() const;
    void shootLaser(const unsigned x, const unsigned y, const bool will_hurt, Invader& owner);
    void handleCollisions(Explosions& explosions, PlayerLaser& player_laser);
    void shootLasers();
    void moveLasers(Explosions& explosions, PlayerLaser& player_laser);
    void removeHitLasers();
    void updateLasers(Explosions& explosions, PlayerLaser& player_laser);

    // Contiene todos los láseres que están disparando actualmente los invasores.
    Lasers lasers;

    // El juego principal puede realizar un seguimiento cuando Invader realmente se movió
    bool move(CoreCannon& cannon, PlayerLaser& player_laser, UFO& ufo, LivesDisplay& lives_disp, Explosions& explosions);

    // Aumenta la velocidad, desciende e invierte la dirección
    void shift();

    void checkHit(PlayerLaser& laser, unsigned& game_score);

public:
    /* Contructor de la clase. */
    InvaderFormation(sf::RenderWindow& window, Textures& textures, Earth& earth, SoundFx& step1, SoundFx& step2, SoundFx& step3, SoundFx& step4, SoundFx& death_snd);

    /* Destructor de la clase. */
    ~InvaderFormation();

    /* Obtiene la lista de láseres disparados por los invasores.*/
    Lasers& getLasers() { return this->lasers; }

    /* Obtiene la matriz 2D de invasores. */
    InvaderVector2D& getInvaders() { return this->invaders; }

    /* Obtiene el número total de invasores en la formación. */
    unsigned getTotal() const { return this->COLUMNS * this->ROWS; }

    /* Obtener el número de invasores eliminados. */
    unsigned getNumKilled() const { return this->num_killed; }

    /* Restablece la formación. */
    void reset(unsigned wave_on = 0);

    /* Actualiza la formación y realiza diversas operaciones. */
    void update(PlayerLaser& laser, CoreCannon& cannon, PlayerLaser& player_laser, UFO& ufo, LivesDisplay& lives_disp, Explosions& explosions, unsigned& game_score);

    /* Dibuja la formación de los invasores en la ventana SFML. */
    void draw(int amount = -1);

    /* Dibuja los láseres disparados por los invasores. */
    void drawLasers();

    /* Establece la altura de la línea de escudo. */
    void setShieldLine(const unsigned shield_line);

    /* Elimina los láseres que han golpeado algún objetivo. */
    void removeLasers();

};

#endif

