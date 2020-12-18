#pragma once

#include <iostream>
#include <vector>

#include "growth.hpp"


struct SimulationSettings
{
    
};

class Simulation
{
    private:
    std::vector<Growth> m_growths;
    
    public:
    Simulation(std::shared_ptr<SimulationSettings>& Setts);

    bool tick();
    
    void draw(sf::RenderTarget& target);
}
