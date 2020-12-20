#pragma once

#include <iostream>
#include <vector>
#include <cmath>
#include <algorithm>
#include <set>
#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>


struct Coords
{
    // Struct for storing coordinates on a hexagonal grid. It stores offset coordinates.
    
    int x;
    int y;

    Coords() {; }
    Coords(int _x, int _y): x(_x), y(_y) {; }

    void print(bool enter=true) const;
    Coords getNeighbour(int direction) const;
};

int modulo(int a, int b);

template <typename T>
T& atCoords(std::vector< std::vector<T> >& data, Coords coords)
{
    return data[coords.x][coords.y];
}

bool operator< (const Coords& a, const Coords& b);
inline bool operator> (const Coords& a, const Coords& b) {return b < a; }
inline bool operator<=(const Coords& a, const Coords& b) {return !(a > b); }
inline bool operator>=(const Coords& a, const Coords& b) {return !(a < b); }

inline bool operator==(const Coords& a, const Coords& b) {return a.x == b.x && a.y == b.y; }
inline bool operator!=(const Coords& a, const Coords& b) {return !(a == b); }

bool inBounds(const Coords& a, int width, int height);

int randomI(int min, int max);

std::vector<int> randomSequence(int min, int max, int length);

template <typename T>
std::set<T> randomlyPick(std::set<T> pickFrom, int toPick)
{
    std::set<T> result;
    std::vector<int> sequence = randomSequence(0, pickFrom.size()-1, toPick);
    int index = 0;
    auto it = pickFrom.begin();

    while(index < sequence.size() && it != pickFrom.end())
    {
	int offset;
	if(index == 0) offset = sequence[index];
	else offset = sequence[index] - sequence[index-1];

	for(int i = 0; i < offset; ++i) ++it;

	result.insert(*it);
	++index;
    }

    return result;
}

void printColor(sf::Color toPrint, bool enter=true);

float colorValue(float point);

sf::Color colorFromRange(float point);
    
std::vector<sf::Color> generatePalette(int colorTotal);
