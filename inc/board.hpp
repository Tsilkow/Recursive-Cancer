#pragma once

#include <iostream>
#include <vector>
#include <memory>
#include <cmath>
#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/Audio.hpp>

#include "commons.hpp"


struct BoardSettings
{
    sf::Vector2i dimensions;
    sf::Vector2i tileSize;
    sf::Color emptyColor;
    sf::Color deadColor;
    sf::Color activeColor;
};

class Board
{
    private:
    std::shared_ptr<BoardSettings> m_bSetts;
    std::vector< std::vector<int> > m_data;
    // -2 = empty (non-growable)
    // -1 = dead
    //  x = organism x, where x > 0
    std::vector<sf::Vertex> m_depiction;
    std::map<int, sf::Color> m_growthColors;
    std::vector<Coords> m_toUpdate;
    std::vector<Coords> m_toRecolor;

    // updates m_depiction
    void update();

    public:
    Board(std::shared_ptr<BoardSettings>& bSetts);

    bool addColor(int id, sf::Color color);
    
    // changes values in m_data
    bool change(Coords at, int newGrowth);

    void tick();

    // draws depiction onto target
    void draw(sf::RenderTarget& target);
};
