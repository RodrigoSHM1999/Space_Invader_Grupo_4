#ifndef EARTH_H
#define EARTH_H

#include <SFML/Graphics.hpp>

class Earth
{
private:
    //Crear atributos positivos
    static constexpr unsigned HEIGHT = 2;//altura de la linea
    const unsigned WIDTH; //ancho de la linea
    static constexpr unsigned X = 0;//posicion x en la ventana
    static constexpr unsigned Y = 660; //posicion y en la ventana
    static constexpr unsigned SPLASH_DMG = 5; // Daño en area
    //Crear objetos de la bibliteca SFML
    sf::Image img; // Creando su imagen
    sf::Texture texture; // Creando su textura
    sf::Sprite sprite; //Creando su sprite

public:
    Earth(const unsigned screenw);

    const sf::Sprite& getSprite() const { return this->sprite; }// Visualizando el sprite

    void reset(); // reiniciando el estado de la linea

    void destroy(const unsigned x); //destruir

};

#endif

