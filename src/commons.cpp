#include "commons.hpp"


void Coords::print(bool enter) const
{
    std::cout << "(" << x << ", " << y << ")";
    if(enter) std::cout << "\n";
}

Coords Coords::getNeighbour(int direction) const 
{
    direction = modulo(direction, 6);

    if(y % 2 == 0)
    {
	switch(direction)
	{
	    case 0: return Coords(x  , y-1); break; // north-east
	    case 1: return Coords(x+1, y  ); break; // east
	    case 2: return Coords(x  , y+1); break; // south-east
	    case 3: return Coords(x-1, y-1); break; // south-west
	    case 4: return Coords(x-1, y  ); break; // west
	    case 5: return Coords(x-1, y+1); break; // north-west
	}
    }
    else
    {
	switch(direction)
	{
	    case 0: return Coords(x+1, y-1); break; // north-east
	    case 1: return Coords(x+1, y  ); break; // east
	    case 2: return Coords(x+1, y+1); break; // south-east
	    case 3: return Coords(x  , y+1); break; // south-west
	    case 4: return Coords(x-1, y  ); break; // west
	    case 5: return Coords(x  , y-1); break; // north-west
	}
    }
}

bool operator<(const Coords& a, const Coords& b)
{
    if(a.y == b.y) return (a.x < b.x);
    else return (a.y < b.y);
}

bool inBounds(const Coords& a, int width, int height)
{
    return (a.x >= 0 && a.x < width && a.y >= 0 && a.y < height);
}

int modulo(int a, int b)
{
    if(a < 0) a += (-a/b+1) * b;
    return a % b;
}

float modulo(float a, float b)
{
    while(a >= b) a -= b;
    while(a <  0) a += b;

    return a;
}
    
int randomI(int min, int max)
{
    if(min >= max) return min;
    return rand() % (max - min + 1) + min;
}

float randomF(float min, float max, float step)
{
    return randomI(std::round(min/step), std::round(max/step)) * step;
}

std::vector<int> randomSequence(int min, int max, int length)
{
    std::vector<int> result;
    int element;
    int toErase = max - min + 1 - length;
    for(int i=min; i<=max; ++i)
    {
	result.push_back(i);
    }
    for(int i=0; i<toErase; ++i)
    {
	element = randomI(0, result.size()-1);
        result.erase(result.begin() + element);
    }
    return result;
}

void printColor(sf::Color toPrint, bool enter)
{
    std::cout << "(" << toPrint.r << ", " << toPrint.g << ", " << toPrint.b << ")";
    if(enter) std::cout << "\n";
}

float colorValue(float point)
{
    point = modulo(point, 1.f);
    
    if(point <= 1.f/6.f) return point*6.f;
    if(point <= 3.f/6.f) return 1.f;
    if(point <= 4.f/6.f) return (4.f/6.f - point)*6.f;
    return 0.f;
}

sf::Color colorFromRange(float point)
{
    return sf::Color(std::round(255.f * colorValue(point + 2.f/6.f)),
		     std::round(255.f * colorValue(point          )),
		     std::round(255.f * colorValue(point - 2.f/6.f)));
}

std::vector<sf::Color> generatePalette(int colorTotal)
{
    std::vector<sf::Color> result;

    for(int i = 0; i < colorTotal; ++i)
    {
	result.emplace_back(colorFromRange((float)i/(float)colorTotal));
    }

    return result;
}
