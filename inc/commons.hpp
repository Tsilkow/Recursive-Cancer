#pragma once

#include <iostream>
#include <vector>
#include <cmath>
#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>


struct Coords
{
    // Struct for storing coordinates on a hexagonal grid. It stores offset coordinates.
    
    int x;
    int y;

    Coords() {; }
    Coords(int _x, int _y): x(_x), y(_y) {; }

    void print(bool enter=true);
    Coords getNeighbour(int direction);
};

int modulo(int a, int b);

template <typename T>
T& atCoords(std::vector< std::vector<T> >& data, Coords coords)
{
    return data[coords.x][coords.y];
}

inline bool operator< (const Coords& a, const Coords& b);
inline bool operator> (const Coords& a, const Coords& b) {return b < a; }
inline bool operator<=(const Coords& a, const Coords& b) {return !(a > b); }
inline bool operator>=(const Coords& a, const Coords& b) {return !(a < b); }

inline bool operator==(const Coords& a, const Coords& b) {return a.x == b.x && a.y == b.y; }
inline bool operator!=(const Coords& a, const Coords& b) {return !(a == b); }

bool inBounds(const Coords& a, int width, int height);

int randomI(int min, int max);

void printColor(sf::Color toPrint, bool enter=true);

float colorValue(float point);

sf::Color colorFromRange(float point);
    
std::vector<sf::Color> generatePalette(int colorTotal);
