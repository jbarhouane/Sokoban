// Copyright[2023] <Jessica Barhouane>
#include <iostream>
#include <fstream>
#include <string>
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include "Sokoban.hpp"

int main(int argc, char* argv[]) {
    std::string level = argv[1];
    Matrix game_level;
    game_level = draw_level(argv[1]);

    std::ifstream _level;
    _level.open(argv[1]);
    int m = 0, n = 0;
    _level >> m >> n;
    std::cout << m << " " << n << std::endl;
    Sokoban game(m, n, game_level);

    sf::RenderWindow window(sf::VideoMode(64 * m, 64 * n), "Sokoban");
    game.setWindow(window);

    sf::Texture texture;
    texture.loadFromFile("Sprite2.sprite3");
    sf::Sprite sprite(texture);

    window.setFramerateLimit(10);
    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }
        }
        window.clear();
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) {
            game.movePlayer(Sokoban::Direction::UP);
        } else if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
            game.movePlayer(Sokoban::Direction::LEFT);
        } else if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {
            game.movePlayer(Sokoban::Direction::DOWN);
        } else if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
            game.movePlayer(Sokoban::Direction::RIGHT);
        }
        window.draw(game);
        if (game.isWon() == true) {
            window.draw(sprite);
        }
        window.display();
    }
    _level.close();

    return 0;
}
