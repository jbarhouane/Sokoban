#pragma once
// Copyright[2023] <Jessica Barhouane>
#include <iostream>
#include <string>
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>

class Matrix {
 public:
    Matrix();
    Matrix(size_t m, size_t n);
    Matrix(const Matrix& a);
    Matrix& operator=(const Matrix& a);
    ~Matrix() { delete[] matrix; }

    size_t height() const { return rows; }
    size_t width() const { return columns; }
    size_t size() const { return height() * width(); }

    char& at(size_t m, size_t n);
    char& operator()(size_t m, size_t n);

    const char& operator()(size_t m, size_t n) const;

 private:
    size_t rows;
    size_t columns;
    char* matrix;
};

Matrix draw_level(std::string level);

class Sokoban : public sf::Drawable {
 public:
    // Constructors and Destructors
    Sokoban(int h, int w, Matrix m);
    ~Sokoban() {
        delete[] sprites;
        delete[] crates;
        delete[] walls;
        delete[] xPoints;
        delete[] yPoints;
        delete window;
    }

    // Accessors
    int getHeight(void) { return height; }
    int getWidth(void) { return width; }

    // Mutators
    void setWindow(sf::RenderWindow& sWindow);
    void setHeight(int n) { height = n; }
    void setWidth(int m) { width = m; }
    void setMatrix(Matrix matrix);

    // Functions
    enum Direction { UP, LEFT, DOWN, RIGHT };
    void movePlayer(Direction direction);
    int detectCollision(int x[], int y[], int size, sf::Sprite sprite);
    int detectBoxCollision(sf::Sprite sprite);
    bool isWon();
    // friend std::ostream& operator<<(std::ostream& out, std::ofstream& outfile);
    // friend std::istream& operator>>(std::istream& in, std::ifstream& infile);

 private:
    sf::RenderWindow* window;
    Matrix level;

    sf::Sprite* sprites;
    sf::Sprite* crates;
    sf::Sprite* walls;
    sf::Sprite* store;

    int* xPoints;
    int* yPoints;
    int* crateXPoints;
    int* crateYPoints;
    int* dropXPoints;
    int* dropYPoints;

    sf::Texture _player;
    sf::Texture _wall;
    sf::Texture _crate;
    sf::Texture _ground;
    sf::Texture _storage;

    sf::Sprite player;
    sf::Sprite wall;
    sf::Sprite crate;
    sf::Sprite ground;
    sf::Sprite storage;

    int height;
    int width;
    int numBoxes;
    int numWalls;

    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const {
        for (int i = 0; i < height; i++) {
            for (int j = 0; j < width; j++) {
                target.draw(sprites[i * height + j], states);
            }
        }
        for (int k = 0; k < numBoxes; k++) {
            target.draw(crates[k], states);
        }
        for (int l = 0; l < numWalls; l++) {
            target.draw(walls[l], states);
        }
        target.draw(player, states);
    }
};
