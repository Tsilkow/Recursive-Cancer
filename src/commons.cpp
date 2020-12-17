#include "commons.hpp"


void Coords::print(bool enter)
{
    std::cout << "(" << x << ", " << y << ")";
    if(enter) std::cout << "\n";
}

Coords Coords::getNeighbour(int direction)
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
    
int randomI(int min, int max)
{
    if(min >= max) return min;
    return rand() % (max - min + 1) + min;
}

void printColor(sf::Color toPrint, bool enter)
{
    std::cout << "(" << toPrint.r << ", " << toPrint.g << ", " << toPrint.b << ")";
    if(enter) std::cout << "\n";
}
