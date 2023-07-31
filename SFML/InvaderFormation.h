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

/* Representa toda la formaci�n de Space Invader.
 * Coordina el movimiento y disparo de todos los Space Invaders. */

typedef std::vector<Invader*> InvaderRow;
typedef std::vector<InvaderRow> InvaderVector2D;
typedef std::vector<InvaderLaser*> Lasers;

class InvaderFormation
{
private:
    // Tama�o de la formaci�n
    static constexpr unsigned COLUMNS = 5;
    static constexpr unsigned ROWS = 11;

    // En donde comienza la formaci�n en la pantalla
    static constexpr unsigned STARTX = 120;
    const unsigned STARTY = 150;

    // El espacio entre cada invasor en la formaci�n
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

    // Tierra, la l�nea en la parte inferior de la pantalla.
    Earth& earth;

    // Cuando move_tick golpea move_tick_max, la formaci�n se mueve
    // move_tick_max se reduce cada vez que la formaci�n toca el borde de la pantalla
    int move_tick;
    int move_tick_max;
    int move_tick_change;

    unsigned step_on;
    unsigned num_killed; // N�mero de invasores asesinados.

    bool has_hit_edge;

    int shot_chance;

    // Sonidos utilizados
    SoundFx& step1;
    SoundFx& step2;
    SoundFx& step3;
    SoundFx& step4;
    SoundFx& death_snd;


    // Verifica si los invasores est�n explotando y aumenta la marca.
    void incDeathTick();

    // Ciclos a trav�s de 4 sonidos diferentes en cada paso.
    void playStepSound();

    // Aumenta la velocidad de disparo al reducir la probabilidad de disparo.
    void increaseFire(const int amount);

    // M�todos privados que se ocupan de los l�seres.
    bool toShootOrNah() const;
    void shootLaser(const unsigned x, const unsigned y, const bool will_hurt, Invader& owner);
    void handleCollisions(Explosions& explosions, PlayerLaser& player_laser);
    void shootLasers();
    void moveLasers(Explosions& explosions, PlayerLaser& player_laser);
    void removeHitLasers();
    void updateLasers(Explosions& explosions, PlayerLaser& player_laser);

    // Contiene todos los l�seres que est�n disparando actualmente los invasores.
    Lasers lasers;

    // El juego principal puede realizar un seguimiento cuando Invader realmente se movi�
    bool move(CoreCannon& cannon, PlayerLaser& player_laser, UFO& ufo, LivesDisplay& lives_disp, Explosions& explosions);

    // Aumenta la velocidad, desciende e invierte la direcci�n
    void shift();

    void checkHit(PlayerLaser& laser, unsigned& game_score);

public:
    /* Contructor de la clase. */
    InvaderFormation(sf::RenderWindow& window, Textures& textures, Earth& earth, SoundFx& step1, SoundFx& step2, SoundFx& step3, SoundFx& step4, SoundFx& death_snd);

    /* Destructor de la clase. */
    ~InvaderFormation();

    /* Obtiene la lista de l�seres disparados por los invasores.*/
    Lasers& getLasers() { return this->lasers; }

    /* Obtiene la matriz 2D de invasores. */
    InvaderVector2D& getInvaders() { return this->invaders; }

    /* Obtiene el n�mero total de invasores en la formaci�n. */
    unsigned getTotal() const { return this->COLUMNS * this->ROWS; }

    /* Obtener el n�mero de invasores eliminados. */
    unsigned getNumKilled() const { return this->num_killed; }

    /* Restablece la formaci�n. */
    void reset(unsigned wave_on = 0);

    /* Actualiza la formaci�n y realiza diversas operaciones. */
    void update(PlayerLaser& laser, CoreCannon& cannon, PlayerLaser& player_laser, UFO& ufo, LivesDisplay& lives_disp, Explosions& explosions, unsigned& game_score);

    /* Dibuja la formaci�n de los invasores en la ventana SFML. */
    void draw(int amount = -1);

    /* Dibuja los l�seres disparados por los invasores. */
    void drawLasers();

    /* Establece la altura de la l�nea de escudo. */
    void setShieldLine(const unsigned shield_line);

    /* Elimina los l�seres que han golpeado alg�n objetivo. */
    void removeLasers();

};

#endif

