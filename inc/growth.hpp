#pragma once

#include <iostream>
#include <vector>
#include <memory>
#include <unordered_set>
#include <unordered_map>

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
    std::unordered_set<Coords> m_territory;
    std::unordered_set<Coords> m_duplicating;
    std::unordered_set<Coords> m_edge;
    std::unordered_map<Coords, int> m_requests;
    std::unordered_set<Coords> m_empty;
    std::unordered_set<Coords> m_dead;
    std::unordered_set<Coords> m_enemy;
    int m_size;
    int m_breathingLength;
    int m_breathGivenAway;
    float m_fluorescence;
    bool m_shrinked = true;
    std::shared_ptr<Growth> m_host;
    
    public:
    Growth(std::shared_ptr<GrowthSettings>& gSetts, Coords heart);

    bool isControlled(const Coords at);

    bool isDuplicating(const Coords at);

    std::unordered_set<Coords> checkConnectedness();
    
    void breathe(std::unordered_set<Coords> empty,
		 std::unordered_set<Coords> dead,
		 std::unordered_set<Coords> enemy);

    void feed();

    std::unordered_set<Coords> getPlan();

    void grow(Coords toAdd);

    bool shrink(Coords toSubtract);

    void request(Coords from, int amount);

    int getRequested(Coords from);

    void setHost(std::shared_ptr<Growth>& host);

    void resetHost();

    const int& getSize() {return m_size; }
    const Coords& getHeart() {return m_heart; }
    const float& getFluorescence() {return m_fluorescence; }
    const std::unordered_set<Coords>& getEdge() {return m_edge; }
};
