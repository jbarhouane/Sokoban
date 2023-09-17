// Copyright[2023] <Jessica Barhouane>

#include <iostream>
#include <algorithm>
#include <stdexcept>
#include <fstream>
#include <vector>
#include <cmath>
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include "Sokoban.hpp"

Matrix::Matrix() : rows(0), columns(0), matrix(nullptr) {}

Matrix::Matrix(size_t m, size_t n) : rows(m), columns(n), matrix(new char[m * n]) {}

Matrix::Matrix(const Matrix& a) : rows(a.rows), columns(a.columns), matrix(new char[a.size()]) {
    std::copy(a.matrix, a.matrix + a.size(), matrix);
}

Matrix& Matrix::operator=(const Matrix& a) {
    if (this != &a) {
        char* data = new char[a.size()];
        delete[] matrix;
        matrix = data;
        rows = a.rows;
        columns = a.columns;
        std::copy(a.matrix, a.matrix + a.size(), matrix);
    }
    return *this;
}

char& Matrix::operator()(size_t m, size_t n) {
    return matrix[m * columns + n];
}

const char& Matrix::operator()(size_t m, size_t n) const {
    return matrix[m * columns + n];
}

char& Matrix::at(size_t m, size_t n) {
    if (m >= rows || n >= columns) {
        throw std::out_of_range("Out of range!");
    }
    return matrix[m * columns + n];
}

Matrix draw_level(std::string level) {
    std::ifstream _level;
    _level.open(level);
    int m, n, i, j;
    char c;
    _level >> m;
    _level >> n;
    Matrix game_level(m, n);
    for (i = 0; i < m; i++) {
        for (j = 0; j < n; j++) {
            _level >> c;
            game_level(i, j) = c;
        }
    }
    _level.close();
    return game_level;
}

// Constructors
Sokoban::Sokoban(int h, int w, Matrix m) {
    height = h;
    width = w;
    level = m;
    numBoxes = 0;
    numWalls = 0;
    int bNum = 0;
    int wNum = 0;
    int sNum = 0;

    _player.loadFromFile("player_05.png");
    _wall.loadFromFile("block_06.png");
    _crate.loadFromFile("crate_03.png");
    _ground.loadFromFile("ground_01.png");
    _storage.loadFromFile("ground_04.png");

    player.setTexture(_player);
    wall.setTexture(_wall);
    crate.setTexture(_crate);
    ground.setTexture(_ground);
    storage.setTexture(_storage);

    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            if (level(i, j) == 'A') {
                numBoxes++;
            }
        }
    }

    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            if (level(i, j) == '#') {
                numWalls++;
            }
        }
    }

    sprites = new sf::Sprite[height * width];
    crates = new sf::Sprite[numBoxes];
    walls = new sf::Sprite[numWalls];
    store = new sf::Sprite[numBoxes];
    xPoints = new int[numWalls];
    yPoints = new int[numWalls];
    crateXPoints = new int[numBoxes];
    crateYPoints = new int[numBoxes];
    dropXPoints = new int[numBoxes];
    dropYPoints = new int[numBoxes];
    


    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            if (level(i, j) == '#') {
                wall.setPosition(64 * i, 64 * j);
                sprites[i * height + j] = ground;
                walls[wNum] = wall;
                xPoints[wNum] = i;
                yPoints[wNum] = j;
                wNum++;
            } else if (level(i, j) == 'a') {
                storage.setPosition(64 * i, 64 * j);
                sprites[i * height + j] = storage;
                store[sNum] = storage;
                dropXPoints[sNum] = i;
                dropYPoints[sNum] = j;
            } else {
                ground.setPosition(64 * i, 64 * j);
                sprites[i * height + j] = ground;
            }
            if (level(i, j) == 'A') {
                ground.setPosition(64 * i, 64 * j);
                sprites[i * height + j] = ground;
                crate.setPosition(64 * i, 64 * j);
                crates[bNum] = crate;
                bNum++;
            }
            if (level(i, j) == '@') {
                player.setPosition(64 * i, 64 * j);
                ground.setPosition(64 * i, 64 * j);
                sprites[i * height + j] = ground;
            }
        }
    }
}



// Setters
void Sokoban::setWindow(sf::RenderWindow& sWindow) {
    window = &sWindow;
}
void Sokoban::setMatrix(Matrix matrix) {
    level = matrix;
}

// Functions
int Sokoban::detectBoxCollision(sf::Sprite sprite) {
    for (int i = 0; i < numBoxes; i++) {
        if (sprite.getGlobalBounds().intersects(crates[i].getGlobalBounds())) {
            return i;
        }
    }
    return -1;
}

void Sokoban::movePlayer(Sokoban::Direction direction) {
    sf::Vector2f oldPos = player.getPosition();
    sf::Vector2f moveOffset;

    switch (direction) {
    case Sokoban::Direction::UP:
        moveOffset = sf::Vector2f(0, -64);
        break;
    case Sokoban::Direction::LEFT:
        moveOffset = sf::Vector2f(-64, 0);
        break;
    case Sokoban::Direction::DOWN:
        moveOffset = sf::Vector2f(0, 64);
        break;
    case Sokoban::Direction::RIGHT:
        moveOffset = sf::Vector2f(64, 0);
        break;
    }

    player.move(moveOffset);
    int wallCollision = detectCollision(xPoints, yPoints, numWalls, player);
    if (wallCollision != -1) {
        player.setPosition(oldPos);
        return;
    }

    int boxCollision = detectBoxCollision(player);
    if (boxCollision != -1) {
        sf::Vector2f oldBoxPos = crates[boxCollision].getPosition();
        crates[boxCollision].move(moveOffset);

        int boxWallCollision = detectCollision(xPoints, yPoints, numWalls, crates[boxCollision]);
        int otherBoxCollision = detectBoxCollision(crates[boxCollision]);

        if (boxWallCollision != -1 || (otherBoxCollision != -1 && otherBoxCollision != boxCollision)) {
            crates[boxCollision].setPosition(oldBoxPos);
            player.setPosition(oldPos);
        }
    }
}


int Sokoban::detectCollision(int x[], int y[], int size, sf::Sprite sprite) {
    for (int i = 0; i < size; i++) {
        if (sprite.getGlobalBounds().intersects(walls[i].getGlobalBounds())) {
            return i;
        }
    }
    return -1;
}

bool Sokoban::isWon() {
    int checkWin = 0;
    for (int i = 0; i < numBoxes; i++) {
        for (int j = 0; j < numBoxes; j++) {


            if (std::abs(crates[i].getPosition().x - store[j].getPosition().x) < 1 && std::abs(crates[i].getPosition().y - store[j].getPosition().y) < 1) {
                checkWin++;
                break;
            }
        }
    }
    return checkWin == numBoxes;
}

