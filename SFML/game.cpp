#include <fstream>
#include <sstream>
#include <cmath>
#include "game.h"
#include "globals.h"

bool Game::life_awarded = false;

void Game::handle_events(sf::Window& window, Menu& menu, ScoreDisplay& score_disp, LivesDisplay& lives_disp, InvaderFormation& invaders, PlayerLaser& player_laser, CoreCannon& cannon, ShieldWall& shields, Earth& earth, UFO& ufo, Explosions& explosions, unsigned& wave_on)
{
    // Maneja los eventos del juego, como el cierre de la ventana.
    sf::Event event;
    while (window.pollEvent(event))
    {
        switch (event.type)
        {
        case sf::Event::Closed:
            window.close();
            break;

        case sf::Event::KeyReleased:
            switch (event.key.code)
            {
            case sf::Keyboard::P:
                pause(ufo);
                break;

            case sf::Keyboard::Escape:
                goto_menu(menu, score_disp, lives_disp, ufo, invaders, shields, cannon, player_laser, earth, explosions, wave_on);
                break;

            default:
                break;
            }

            break;

        default:
            break;
        }
    }
}

void Game::real_time_key(InvaderFormation& invaders, PlayerLaser& player_laser, CoreCannon& cannon, ShieldWall& shields, Earth& earth, UFO& ufo, Explosions& explosions, unsigned& wave_on)
{
    // Si el juego está pausado o en la configuración de la oleada o en el estado GAME_OVER, no se maneja la entrada del teclado.
    if (Globals::GAME_STATE == Globals::States::PAUSED || Globals::GAME_STATE == Globals::States::WAVE_SETUP || Globals::GAME_STATE == Globals::States::GAME_OVER)
        return;

    if (Globals::GAME_STATE == Globals::States::MENU)
    {
        // Si el juego está en el menú, se verifica si el jugador presiona la tecla Enter para comenzar a jugar.
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Return))
            play_game(invaders, player_laser, cannon, shields, earth, ufo, explosions, wave_on);

        return;
    }

    // Mueve el cañón del jugador hacia la derecha o hacia la izquierda según la tecla presionada.
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
        cannon.move(1);
    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
        cannon.move(-1);

    // Dispara el láser del jugador si el cañón no está dañado y el láser no está en movimiento.
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space) && !cannon.isHit() && !player_laser.isShooting())
    {
        sf::Vector2f cannonpos = cannon.getSprite().getPosition();
        player_laser.shoot(cannonpos.x, cannonpos.y, ufo);
    }
}

void Game::draw_player_laser(sf::RenderWindow& window, PlayerLaser& laser)
{
    // Dibuja el láser del jugador si está en movimiento.
    if (laser.isShooting())
        window.draw(laser.getShape());
}

void Game::update_objects(CoreCannon& cannon, PlayerLaser& player_laser, Earth& earth, UFO& ufo, InvaderFormation& invaders, ShieldWall& shields, LivesDisplay& lives_disp, Explosions& explosions, unsigned& game_score, unsigned& wave_on)
{
    // No hace nada si el juego está pausado, en GAME_OVER o en la configuración de la oleada (o en el menú).
    if (Globals::GAME_STATE == Globals::States::PAUSED || Globals::GAME_STATE == Globals::States::WAVE_SETUP || Globals::GAME_STATE == Globals::States::GAME_OVER)
        return;

    if (Globals::GAME_STATE == Globals::States::MENU)
    {
        // Si el juego está en el menú, solo se maneja la entrada del teclado en tiempo real.
        real_time_key(invaders, player_laser, cannon, shields, earth, ufo, explosions, wave_on);
        return;
    }

    if (Globals::GAME_STATE != Globals::States::PLAYER_KILLED)
    {
        // Si el jugador no ha sido eliminado, se maneja la entrada del teclado en tiempo real y se actualizan los objetos del juego.
        real_time_key(invaders, player_laser, cannon, shields, earth, ufo, explosions, wave_on);
        player_laser.move(explosions);
        ufo.update(player_laser, game_score);
        invaders.update(player_laser, cannon, player_laser, ufo, lives_disp, explosions, game_score);
        shields.handleCollisions(player_laser, invaders.getLasers(), invaders, explosions);
        explosions.update();
    }

    // Actualiza el cañón del jugador para controlar su estado y movimiento.
    cannon.update(invaders, player_laser, ufo, lives_disp, explosions);

    // Si se eliminaron todos los invasores en la formación actual, se configura una nueva oleada.
    if (invaders.getNumKilled() == invaders.getTotal())
        setup_wave(invaders, player_laser, cannon, shields, earth, ufo, explosions, wave_on);

    // Si la puntuación del juego alcanza 1000 y aún no se otorga una vida extra, se otorga una vida adicional.
    if (game_score >= 1000 && !life_awarded)
    {
        life_awarded = true;
        lives_disp.addLife();
    }

    // Limita la puntuación del juego a un máximo de 10,000.
    if (game_score > 10000)
        game_score = 0;
}

void Game::draw_text(sf::RenderWindow& window, const std::string msg, const unsigned x, const unsigned y, sf::Color color, unsigned size)
{
    // Dibuja texto en la ventana con la fuente específica utilizada en el juego, en la posición dada.
    sf::Font font;
    font.loadFromFile(Globals::FONTS_PATH + "space_invaders.ttf");

    sf::Text text;
    text.setFont(font);
    text.setString(msg);
    text.setCharacterSize(size);
    text.setFillColor(color);
    text.setPosition(x, y);

    window.draw(text);
}

void Game::draw_objects(sf::RenderWindow& window, Menu& menu, ScoreDisplay& score_disp, LivesDisplay& lives_disp, UFO& ufo, InvaderFormation& invaders, ShieldWall& shields, CoreCannon& cannon, PlayerLaser& playerlaser, Earth& earth, Explosions& explosions, unsigned& wave_on)
{
    window.clear(Globals::BG_COLOR);

    // Si el juego está en el menú, solo se dibuja el menú y no se muestra el resto de objetos.
    if (Globals::GAME_STATE == Globals::States::MENU)
    {
        menu.draw(window);
        window.display();
        return;
    }

    // Dibuja los objetos del juego en la ventana.

    // Dibuja el puntaje en pantalla.
    score_disp.draw(window);

    // Dibuja la nave enemiga (UFO).
    ufo.draw(window);

    // Dibuja los láseres de los invasores enemigos.
    invaders.drawLasers();

    // Dibuja los muros protectores.
    shields.draw();

    // Dibuja el láser del jugador si está en movimiento.
    Game::draw_player_laser(window, playerlaser);

    // Dibuja la tierra (objetivo del jugador).
    window.draw(earth.getSprite());

    // Dibuja la cantidad de vidas del jugador en pantalla.
    lives_disp.draw(window);

    // Dibuja las explosiones en pantalla.
    explosions.draw(window);

    // Si se está configurando una nueva oleada, se dibujan los invasores de a uno por vez con un pequeño retraso.
    if (Globals::GAME_STATE != Globals::States::WAVE_SETUP)
    {
        if (Globals::GAME_STATE != Globals::States::GAME_OVER)
            window.draw(cannon.getSprite());

        invaders.draw();
    }
    else
        draw_wave(invaders);

    // Si el juego está en GAME_OVER, se maneja el estado de "Game Over".
    if (Globals::GAME_STATE == Globals::States::GAME_OVER)
        handle_game_over(window, menu, score_disp, lives_disp, ufo, invaders, shields, cannon, playerlaser, earth, explosions, wave_on);

    window.display();
}

void Game::updateFPS(sf::Window& window, const sf::Clock& fps_clock, float& fps_timer)
{
    // Calcula los cuadros por segundo (FPS) y actualiza el título de la ventana con ese valor.
    float fps = 1.0f / (fps_clock.getElapsedTime().asSeconds() - fps_timer);
    std::ostringstream title;
    title << Globals::SCREEN_TITLE << " - " << round(fps) << "fps";
    window.setTitle(title.str());
    fps_timer = fps_clock.getElapsedTime().asSeconds();
}

void Game::pause(UFO& ufo)
{
    // Pausa o reanuda el juego según el estado actual del juego (pausado o en juego).
    if (Globals::GAME_STATE == Globals::States::PAUSED)
        Globals::GAME_STATE = Globals::States::PLAY;
    else
        Globals::GAME_STATE = Globals::States::PAUSED;

    // Pausa o reanuda los movimientos del UFO según el estado del juego.
    ufo.pause();
}

void Game::handle_player_kill(InvaderFormation& invaders, PlayerLaser& player_laser, UFO& ufo, Explosions& explosions)
{
    // Maneja la eliminación del jugador.

    // Detiene los láseres de los invasores.
    invaders.removeLasers();

    // Detiene el láser del jugador.
    player_laser.stop();

    // Pausa los movimientos del UFO.
    ufo.pause();

    // Resetea las explosiones.
    explosions.reset();

    // Establece el estado del juego a "PLAYER_KILLED".
    Globals::GAME_STATE = Globals::States::PLAYER_KILLED;
}

void Game::setup_wave(InvaderFormation& invaders, PlayerLaser& player_laser, CoreCannon& cannon, ShieldWall& shields, Earth& earth, UFO& ufo, Explosions& explosions, unsigned& wave_on, bool start_game)
{
    // Configura una nueva oleada de invasores.

    Globals::GAME_STATE = Globals::States::WAVE_SETUP;

    // Si el juego acaba de comenzar, no se incrementa el número de oleada.
    if (!start_game)
        ++wave_on;

    // Resetea la formación de invasores según la oleada actual.
    if (wave_on == 5)
        wave_on = 0;

    // Reinicia la formación de invasores.
    invaders.reset(wave_on);

    // Detiene el láser del jugador.
    player_laser.stop();

    // Resetea el cañón del jugador.
    cannon.reset();

    // Resetea los muros protectores.
    shields.reset();

    // Resetea la tierra (objetivo del jugador).
    earth.reset();

    // Resetea el UFO.
    ufo.reset();

    // Resetea las explosiones.
    explosions.reset();
}

void Game::draw_wave(InvaderFormation& invaders)
{
    // Dibuja los invasores de a uno por vez en pantalla con un pequeño retraso.

    // Bit de código un poco improvisado, pero parece ser el método más sencillo para lograr el efecto de dibujar los invasores uno a la vez.
    static unsigned amount = 1;
    invaders.draw(amount);
    ++amount;

    // Hace una pausa breve antes de dibujar el siguiente invasor.
    // La pausa está bien aquí porque no ocurre nada más mientras se dibujan los invasores.
    sf::sleep(sf::milliseconds(10));

    // Si se dibujaron todos los invasores, se reinicia el contador y se cambia el estado del juego a "PLAY".
    if (amount == (invaders.getTotal() + 1))
    {
        amount = 1;
        Globals::GAME_STATE = Globals::States::PLAY;
    }
}

void Game::handle_game_over(sf::RenderWindow& window, Menu& menu, ScoreDisplay& score_disp, LivesDisplay& lives_disp, UFO& ufo, InvaderFormation& invaders, ShieldWall& shields, CoreCannon& cannon, PlayerLaser& playerlaser, Earth& earth, Explosions& explosions, unsigned& wave_on)
{
    // Maneja el estado de "GAME_OVER".

    // Resetea el UFO.
    ufo.reset();

    // Mensaje a mostrar en pantalla para "GAME OVER".
    std::string msg = "GAME OVER ";
    static unsigned char_on = 0;

    std::ostringstream ss;
    for (unsigned i = 0; i <= char_on; ++i)
    {
        ss << msg[i];
        ss << " ";
    }

    // Dibuja el mensaje de "GAME OVER" en pantalla con un pequeño efecto.
    draw_text(window, ss.str(), 280, 100, sf::Color::Green, 32);

    // Si se ha mostrado completamente el mensaje, espera unos segundos, reinicia el contador y vuelve al menú.
    if (char_on == msg.length() - 1)
    {
        sf::sleep(sf::seconds(2));
        char_on = 0;
        goto_menu(menu, score_disp, lives_disp, ufo, invaders, shields, cannon, playerlaser, earth, explosions, wave_on);
        return;
    }
    else
        ++char_on;

    // Hace una pausa breve para el efecto de aparición del mensaje.
    sf::sleep(sf::milliseconds(300));
}

void Game::reset_game(Menu& menu, ScoreDisplay& score_disp, LivesDisplay& lives_disp, UFO& ufo, InvaderFormation& invaders, ShieldWall& shields, CoreCannon& cannon, PlayerLaser& playerlaser, Earth& earth, Explosions& explosions, unsigned& wave_on)
{
    // Reinicia el juego al estado inicial.

    // Reinicia el indicador de vida otorgada.
    life_awarded = false;

    // Reinicia el número de oleada de invasores.
    wave_on = 0;

    // Resetea la formación de invasores.
    invaders.reset(wave_on);

    // Detiene el láser del jugador.
    playerlaser.stop();

    // Resetea el cañón del jugador.
    cannon.reset();

    // Resetea los muros protectores.
    shields.reset();

    // Resetea la tierra (objetivo del jugador).
    earth.reset();

    // Resetea el UFO.
    ufo.reset();

    // Resetea el menú.
    menu.reset();

    // Resetea el puntaje.
    score_disp.reset();

    // Resetea la cantidad de vidas del jugador.
    lives_disp.reset();

    // Resetea las explosiones.
    explosions.reset();
}

void Game::goto_menu(Menu& menu, ScoreDisplay& score_disp, LivesDisplay& lives_disp, UFO& ufo, InvaderFormation& invaders, ShieldWall& shields, CoreCannon& cannon, PlayerLaser& playerlaser, Earth& earth, Explosions& explosions, unsigned& wave_on)
{
    // Vuelve al menú principal desde cualquier estado del juego.

    // Reinicia el juego al estado inicial.
    reset_game(menu, score_disp, lives_disp, ufo, invaders, shields, cannon, playerlaser, earth, explosions, wave_on);

    // Cambia el estado del juego a "MENU".
    Globals::GAME_STATE = Globals::States::MENU;
}

void Game::play_game(InvaderFormation& invaders, PlayerLaser& player_laser, CoreCannon& cannon, ShieldWall& shields, Earth& earth, UFO& ufo, Explosions& explosions, unsigned& wave_on)
{
    // Comienza una nueva partida, configurando la primera oleada de invasores.

    // Configura una nueva oleada y comienza el juego.
    setup_wave(invaders, player_laser, cannon, shields, earth, ufo, explosions, wave_on, true);
}

unsigned Game::get_highscore()
{
    // Obtiene el puntaje más alto registrado en el archivo "highscore.txt".

    std::string highscore_txt;
    std::ifstream hs_file(Globals::HS_FILE);

    if (hs_file.is_open())
    {
        std::getline(hs_file, highscore_txt);
        hs_file.close();
    }
    // Si el archivo no existe, se establece el puntaje más alto en 0.
    else
        highscore_txt = "0";

    // Si el puntaje obtenido está vacío o contiene un puntaje no válido, se establece el puntaje más alto en 0.
    if (highscore_txt.empty())
        highscore_txt = "0";
    else
    {
        for (auto c : highscore_txt)
        {
            if (!std::isdigit(c))
            {
                highscore_txt = "0";
                break;
            }
        }
    }

    unsigned highscore = std::stoi(highscore_txt);

    // Un puntaje de 10000 o más es considerado no válido, por lo que se establece en 0.
    return highscore < 10000 ? std::stoi(highscore_txt) : 0;
}

void Game::write_highscore(const unsigned score)
{
    // Escribe el puntaje más alto en el archivo "highscore.txt".

    std::ofstream hs_file(Globals::HS_FILE, std::ios::trunc);

    if (hs_file.is_open())
        hs_file << score;

    hs_file.close();
}