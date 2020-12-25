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
    sf::Color heartColor;
};

class Board
{
    private:
    std::shared_ptr<BoardSettings> m_bSetts;
    std::vector<sf::Vertex> m_depiction;
    std::map<int, sf::Color> m_growthColors;
    std::vector<std::pair<Coords, int>> m_toRecolorNow;
    std::vector<std::pair<Coords, int>> m_toRecolorNext;

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
