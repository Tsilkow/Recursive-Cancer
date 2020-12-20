#pragma once

#include <iostream>
#include <vector>
#include <memory>

#include "commons.hpp"


struct GrowthSettings
{
    float airPerSynthesis;
};

class Growth
{
    private:
    std::shared_ptr<GrowthSettings> m_gSetts;
    Coords m_heart;
    std::set<Coords> m_territory;
    std::set<Coords> m_duplicating;
    std::set<Coords> m_edge;
    int m_size;
    int m_breathingLength;
    float m_fluorescence;
    bool m_shrinked = true;
    
    public:
    Growth(std::shared_ptr<GrowthSettings>& gSetts, Coords heart);

    bool isControlled(const Coords at);

    bool isDuplicating(const Coords at);

    std::set<Coords> checkConnectedness();

    std::set<Coords> tick(std::set<Coords> empty, std::set<Coords> dead, std::set<Coords> enemy);

    void grow(Coords toAdd);

    bool shrink(Coords toSubtract);

    const Coords& getHeart() {return m_heart; }
    const float& getFluorescence() {return m_fluorescence; }
    const std::set<Coords>& getEdge() {return m_edge; }
};
