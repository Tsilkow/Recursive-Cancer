#pragma once

#include <iostream>
#include <vector>
#include <unordered_set>
#include <unordered_map>

#include "commons.hpp"
#include "board.hpp"
#include "growth.hpp"


struct SimulationSettings
{
    std::shared_ptr<GrowthSettings> gSetts;
    std::shared_ptr<BoardSettings> bSetts;
    int colorTotal;
    int colorWalk;
    int cancerImmunity;
    float cancerProbability;
    std::vector<Coords> start;
};

class Simulation
{
    private:
    std::shared_ptr<SimulationSettings> m_sSetts;
    std::unordered_map<int, std::shared_ptr<Growth>> m_growths;
    std::vector< std::vector<int> > m_control;
    std::unordered_multimap<int, std::pair<int, int>> m_cancerImmunity;
    // -2 = empty (non-growable)
    // -1 = dead
    //  x = organism x, where x > 0
    Board m_board;
    int m_nextId;

    void setCell(Coords at, int to, bool heart=false);

    void newCancer(Coords at, int mutatedFrom = -1);
    
    public:
    Simulation(std::shared_ptr<SimulationSettings>& sSetts);

    bool tick();
    
    void draw(sf::RenderTarget& target);
};
