#include <iostream>
#include "Earth.h"

Earth::Earth(const unsigned screenw) : WIDTH(screenw) // constructor
{
    this->reset();
    this->sprite.setPosition(this->X, this->Y);
}

void Earth::reset()
{
    this->img.create(this->WIDTH, this->HEIGHT, sf::Color::Green); //Inicializa su tamaño,su ancho y su color 
    this->texture.loadFromImage(this->img);
    this->sprite.setTexture(this->texture);
}

void Earth::destroy(const unsigned x)
{
    // Uso de sistemas de coordenadas
    unsigned local_x = x - this->X;

    // How many pixels to 'destroy' depends on if x is even or odd
    // This is to keep at least every other pixel showing
    if (!(local_x % 2))
    {
        for (unsigned y = 0; y < this->HEIGHT; ++y)
        {
            for (unsigned i = 0; i < this->SPLASH_DMG; ++i)
                this->img.setPixel(x + i, y, sf::Color::Transparent);
        }
    }
    else
    {
        for (unsigned y = 0; y < this->HEIGHT; ++y)
        {
            for (unsigned i = 0; i < this->SPLASH_DMG; ++i)
            {
                this->img.setPixel((x - this->SPLASH_DMG) + i, y, sf::Color::Transparent);
                this->img.setPixel((x + this->SPLASH_DMG) + i, y, sf::Color::Transparent);
            }
        }
    }

    this->texture.update(this->img);
}