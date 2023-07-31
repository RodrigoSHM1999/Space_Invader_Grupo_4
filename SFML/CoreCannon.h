#ifndef PLAYERcannon_H
#define PLAYERcannon_H

#include <SFML/Graphics.hpp>
#include "SoundFx.h"
#include "Textures.h"
#include "InvaderFormation.h"
#include "Explosions.h"

/* La nave del jugador. */

class CoreCannon
{
private:
    SoundFx& killed_sound;

    const unsigned START_X; // Posición inicial en el eje X de la nave del jugador
    static constexpr unsigned Y = 623; // Posición en el eje Y de la nave del jugador

    static constexpr int SPEED = 5; // Velocidad de movimiento del cañón
    static constexpr unsigned DEATH_TICK_MAX = 50; // Número máximo de "ticks" durante la animación de muerte
    static constexpr unsigned FRAME_TICK_MAX = 3; // Número máximo de "ticks" para cambiar el cuadro de animación

    Textures& textures; // Referencia a las texturas necesarias para la nave del jugador
    sf::Sprite sprite; // Sprite que representa visualmente la nave del jugador

    unsigned frame_on; // Cuadro actual de la animación de la nave
    bool hit; // Indicador booleano que representa si la nave ha sido golpeada
    unsigned death_tick; // Contador para controlar el tiempo de animación de muerte de la nave
    unsigned frame_tick; // Contador para controlar el tiempo de cambio de cuadros de animación

    // Función privada para cambiar el cuadro de animación de la nave cuando es golpeada
    void flipFrame();

public:
    // Constructor que recibe referencias a las texturas y el sonido de muerte
    CoreCannon(Textures& textures, SoundFx& killed_sound, const int startx);

    // Devuelve si la nave ha sido golpeada o no
    bool isHit() const { return this->hit; }

    // Restablece la nave a su estado inicial
    void reset();

    // Mueve la nave en la dirección dada
    void move(const int dir);

    // Devuelve el sprite de la nave
    const sf::Sprite& getSprite() const { return this->sprite; }

    // Devuelve la posición en el eje X de la nave
    unsigned getX() const { return this->sprite.getPosition().x; }

    // Devuelve el ancho del sprite de la nave
    unsigned getWidth() const { return this->getSprite().getGlobalBounds().width; }

    // Maneja la muerte de la nave cuando es golpeada por un invader
    void die(InvaderFormation& invaders, PlayerLaser& player_laser, UFO& ufo, Explosions& explosions);

    // Maneja las colisiones con los lasers de los invaders
    void handleHit(InvaderFormation& invaders, PlayerLaser& player_laser, UFO& ufo, Explosions& explosions);

    // Actualiza el estado de la nave en cada frame
    void update(InvaderFormation& invaders, PlayerLaser& player_laser, UFO& ufo, LivesDisplay& lives_disp, Explosions& explosions);

};

#endif
