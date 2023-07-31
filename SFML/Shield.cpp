#include "Shield.h"

// La clase "Shield" representa los escudos que protegen al jugador del fuego entrante.

// El constructor de "Shield" recibe una referencia a "Textures", las coordenadas (x, y) para posicionar el escudo y asigna la textura correspondiente a la variable "sprite".
Shield::Shield(Textures& textures, const int xpos, const int ypos) : textures(textures)
{
    // Se restablece el escudo y se posiciona en las coordenadas recibidas.
    this->reset();
    this->sprite.setPosition(xpos, ypos);
}

// La funci�n "reset" restablece el escudo. Carga la imagen del escudo desde la textura "SHIELD" y actualiza el sprite con esa imagen.
void Shield::reset()
{
    this->img = this->textures.SHIELD.copyToImage();
    this->texture.loadFromImage(img);
    this->sprite.setTexture(this->texture);
}

// La funci�n "handleCollide" maneja las colisiones de los l�seres con el escudo.
void Shield::handleCollide(PlayerLaser& player_laser, Lasers& invader_lasers, Explosions& explosions)
{
    // TODO: Hacer el c�digo DRYer encontrando una forma de no repetir el bucle FOR de p�xeles.

    // Obtiene el rect�ngulo del escudo para realizar la detecci�n de colisiones.
    sf::FloatRect shield_rect = this->sprite.getGlobalBounds();
    unsigned xpos = this->sprite.getPosition().x;
    unsigned ypos = this->sprite.getPosition().y;
    unsigned width = shield_rect.width;
    unsigned height = shield_rect.height;

    // Verifica las colisiones con los l�seres de los invasores.
    for (auto& laser : invader_lasers)
    {
        // Primero verifica si el rect�ngulo del l�ser colisiona con el rect�ngulo del escudo.
        if (laser->checkCollide(shield_rect))
        {
            // Luego verifica si alg�n p�xel del l�ser colisiona con el escudo y si es verde (color del l�ser).
            for (unsigned x = xpos; x < (xpos + width); ++x)
            {
                for (unsigned y = ypos; y < (ypos + height); ++y)
                {
                    // Resta xpos e ypos porque la imagen utiliza sus propias coordenadas locales.
                    sf::Color pixel = this->img.getPixel(x - xpos, y - ypos);

                    // Si el p�xel es verde y colisiona con el l�ser, crea una explosi�n, elimina el l�ser y da�a el escudo.
                    if (pixel.g > 0 && laser->checkCollide(x, y))
                    {
                        explosions.newExplosion(sf::Color::Green, laser->getX(), laser->getY());
                        laser->remove();
                        this->damageShield(x - xpos, y - ypos, laser->getDmg());
                        break; // No es necesario seguir buscando despu�s del primer impacto.
                    }
                }
            }
        }
    }

    // Verifica si el l�ser del jugador colisiona con el escudo.
    sf::FloatRect player_laser_rect = player_laser.getShape().getGlobalBounds();
    if (player_laser_rect.intersects(shield_rect))
    {
        // Verifica si alg�n p�xel del l�ser del jugador colisiona con el escudo y si es verde.
        for (unsigned x = xpos; x < (xpos + width); ++x)
        {
            for (unsigned y = ypos; y < (ypos + height); ++y)
            {
                // Resta xpos e ypos porque la imagen utiliza sus propias coordenadas locales.
                sf::Color pixel = this->img.getPixel(x - xpos, y - ypos);

                // Si el p�xel es verde y colisiona con el l�ser del jugador, da�a el escudo y detiene el l�ser.
                if (pixel.g > 0 && player_laser_rect.contains(x, y))
                {
                    this->damageShield(x - xpos, y - ypos, player_laser.getDmg());
                    player_laser.stop();
                }
            }
        }
    }
}

// La funci�n "handleCollideInvaders" maneja las colisiones de los invasores con el escudo.
void Shield::handleCollideInvaders(InvaderFormation& invaders)
{
    // Obtiene el rect�ngulo del escudo para realizar la detecci�n de colisiones.
    sf::FloatRect shield_rect = this->sprite.getGlobalBounds();
    unsigned xpos = this->sprite.getPosition().x;
    unsigned ypos = this->sprite.getPosition().y;
    unsigned width = shield_rect.width;
    unsigned height = shield_rect.height;

    // Verifica las colisiones con los invasores.
    for (auto& invader_row : invaders.getInvaders())
    {
        for (auto& invader : invader_row)
        {
            // Primero verifica si el rect�ngulo del invasor colisiona con el rect�ngulo del escudo.
            if (invader->getSprite().getGlobalBounds().intersects(shield_rect))
            {
                // Luego verifica si alg�n p�xel del invasor colisiona con el escudo y si es verde (color del invasor).
                for (unsigned x = xpos; x < (xpos + width); ++x)
                {
                    for (unsigned y = ypos; y < (ypos + height); ++y)
                    {
                        // Resta xpos e ypos porque la imagen utiliza sus propias coordenadas locales.
                        sf::Color pixel = this->img.getPixel(x - xpos, y - ypos);

                        // Si el p�xel es verde y colisiona con el invasor, lo "elimina" del escudo simulando la destrucci�n.
                        if (pixel.g > 0 && invader->getSprite().getGlobalBounds().contains(x, y))
                            this->img.setPixel(x - xpos, y - ypos, sf::Color::Transparent);
                    }
                }
            }
        }
    }

    // Actualiza la textura del escudo con los cambios en la imagen.
    this->texture.update(img);
}

// La funci�n "damageShield" agrega da�o al escudo en la posici�n (x, y) con el valor "dmg".
void Shield::damageShield(int x, int y, const int dmg)
{
    int oldx = x;
    int oldy = y;

    for (x -= dmg; x < (oldx + dmg); ++x)
    {
        // Convierte x e y en unsigned para eliminar la advertencia.
        // Inicialmente son int porque podr�an ser menores que 0, pero la evaluaci�n de cortocircuito asegura que no sean negativos cuando se comparan con los resultados de getSize().
        if (x < 0 || static_cast<unsigned>(x) >= this->img.getSize().x)
            continue;

        for (y -= dmg; y < (oldy + dmg); ++y)
        {
            if (y < 0 || static_cast<unsigned>(y) >= this->img.getSize().y)
                continue;

            // Actualiza el p�xel en la posici�n (x, y) con el color transparente para simular la destrucci�n del escudo.
            this->img.setPixel(x, y, sf::Color::Transparent);
        }
    }

    // Actualiza la textura del escudo con los cambios en la imagen.
    this->texture.update(img);
}

