#ifndef SOUNDFX_H
#define SOUNDFX_H
#include <string>
#include <SFML/Audio.hpp>

//Clase para agregar el sonido
class SoundFx
{
private:
    sf::Sound sound;
    sf::SoundBuffer buffer;

public:
    SoundFx(std::string filename);//inicializar con el archivo del sonido
    sf::SoundSource::Status getStatus() { return sound.getStatus(); }
    void play(bool loop = false);
    void pause();
    void stop();
    void setVolume(float volume);

};

#endif