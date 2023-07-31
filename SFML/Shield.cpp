#include "Shield.h"

// La clase "Shield" representa los escudos que protegen al jugador del fuego entrante.

// El constructor de "Shield" recibe una referencia a "Textures", las coordenadas (x, y) para posicionar el escudo y asigna la textura correspondiente a la variable "sprite".
Shield::Shield(Textures& textures, const int xpos, const int ypos) : textures(textures)
{
    // Se restablece el escudo y se posiciona en las coordenadas recibidas.
    this->reset();
    this->sprite.setPosition(xpos, ypos);
}

// La función "reset" restablece el escudo. Carga la imagen del escudo desde la textura "SHIELD" y actualiza el sprite con esa imagen.
void Shield::reset()
{
    this->img = this->textures.SHIELD.copyToImage();
    this->texture.loadFromImage(img);
    this->sprite.setTexture(this->texture);
}

// La función "handleCollide" maneja las colisiones de los láseres con el escudo.
void Shield::handleCollide(PlayerLaser& player_laser, Lasers& invader_lasers, Explosions& explosions)
{
    // TODO: Hacer el código DRYer encontrando una forma de no repetir el bucle FOR de píxeles.

    // Obtiene el rectángulo del escudo para realizar la detección de colisiones.
    sf::FloatRect shield_rect = this->sprite.getGlobalBounds();
    unsigned xpos = this->sprite.getPosition().x;
    unsigned ypos = this->sprite.getPosition().y;
    unsigned width = shield_rect.width;
    unsigned height = shield_rect.height;

    // Verifica las colisiones con los láseres de los invasores.
    for (auto& laser : invader_lasers)
    {
        // Primero verifica si el rectángulo del láser colisiona con el rectángulo del escudo.
        if (laser->checkCollide(shield_rect))
        {
            // Luego verifica si algún píxel del láser colisiona con el escudo y si es verde (color del láser).
            for (unsigned x = xpos; x < (xpos + width); ++x)
            {
                for (unsigned y = ypos; y < (ypos + height); ++y)
                {
                    // Resta xpos e ypos porque la imagen utiliza sus propias coordenadas locales.
                    sf::Color pixel = this->img.getPixel(x - xpos, y - ypos);

                    // Si el píxel es verde y colisiona con el láser, crea una explosión, elimina el láser y daña el escudo.
                    if (pixel.g > 0 && laser->checkCollide(x, y))
                    {
                        explosions.newExplosion(sf::Color::Green, laser->getX(), laser->getY());
                        laser->remove();
                        this->damageShield(x - xpos, y - ypos, laser->getDmg());
                        break; // No es necesario seguir buscando después del primer impacto.
                    }
                }
            }
        }
    }

    // Verifica si el láser del jugador colisiona con el escudo.
    sf::FloatRect player_laser_rect = player_laser.getShape().getGlobalBounds();
    if (player_laser_rect.intersects(shield_rect))
    {
        // Verifica si algún píxel del láser del jugador colisiona con el escudo y si es verde.
        for (unsigned x = xpos; x < (xpos + width); ++x)
        {
            for (unsigned y = ypos; y < (ypos + height); ++y)
            {
                // Resta xpos e ypos porque la imagen utiliza sus propias coordenadas locales.
                sf::Color pixel = this->img.getPixel(x - xpos, y - ypos);

                // Si el píxel es verde y colisiona con el láser del jugador, daña el escudo y detiene el láser.
                if (pixel.g > 0 && player_laser_rect.contains(x, y))
                {
                    this->damageShield(x - xpos, y - ypos, player_laser.getDmg());
                    player_laser.stop();
                }
            }
        }
    }
}

// La función "handleCollideInvaders" maneja las colisiones de los invasores con el escudo.
void Shield::handleCollideInvaders(InvaderFormation& invaders)
{
    // Obtiene el rectángulo del escudo para realizar la detección de colisiones.
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
            // Primero verifica si el rectángulo del invasor colisiona con el rectángulo del escudo.
            if (invader->getSprite().getGlobalBounds().intersects(shield_rect))
            {
                // Luego verifica si algún píxel del invasor colisiona con el escudo y si es verde (color del invasor).
                for (unsigned x = xpos; x < (xpos + width); ++x)
                {
                    for (unsigned y = ypos; y < (ypos + height); ++y)
                    {
                        // Resta xpos e ypos porque la imagen utiliza sus propias coordenadas locales.
                        sf::Color pixel = this->img.getPixel(x - xpos, y - ypos);

                        // Si el píxel es verde y colisiona con el invasor, lo "elimina" del escudo simulando la destrucción.
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

// La función "damageShield" agrega daño al escudo en la posición (x, y) con el valor "dmg".
void Shield::damageShield(int x, int y, const int dmg)
{
    int oldx = x;
    int oldy = y;

    for (x -= dmg; x < (oldx + dmg); ++x)
    {
        // Convierte x e y en unsigned para eliminar la advertencia.
        // Inicialmente son int porque podrían ser menores que 0, pero la evaluación de cortocircuito asegura que no sean negativos cuando se comparan con los resultados de getSize().
        if (x < 0 || static_cast<unsigned>(x) >= this->img.getSize().x)
            continue;

        for (y -= dmg; y < (oldy + dmg); ++y)
        {
            if (y < 0 || static_cast<unsigned>(y) >= this->img.getSize().y)
                continue;

            // Actualiza el píxel en la posición (x, y) con el color transparente para simular la destrucción del escudo.
            this->img.setPixel(x, y, sf::Color::Transparent);
        }
    }

    // Actualiza la textura del escudo con los cambios en la imagen.
    this->texture.update(img);
}

