#pragma once

#include <iostream>
#include <vector>

#include "board.hpp"


struct SimulationSettings
{
    std::shared_ptr<BoardSettings> bSetts;
    int colorTotal;
};

class Simulation
{
    private:
    std::shared_ptr<SimulationSettings> m_sSetts;
    //std::vector<Growth> m_growths;
    Board m_board;
    
    public:
    Simulation(std::shared_ptr<SimulationSettings>& sSetts);

    bool tick();
    
    void draw(sf::RenderTarget& target);
};
