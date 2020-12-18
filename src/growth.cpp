#include "growth.hpp"


Growth::Growth(std::shared_ptr<GrowthSettings>& gSetts, Coords heart):
    m_gSetts(gSetts),
    m_heart(heart),
    m_size(1),
    m_breathingLength(0),
    m_fluorescence(1.f)
{
    m_territory.insert(heart);
}

bool Growth::isControlled(const Coords at)
{
    return (m_territory.find(at) != m_territory.end());
}

std::set<Coords> Growth::getEdge()
{
    std::set<Coords> result;

    for(auto it = m_territory.begin(); it != m_territory.end(); ++it)
    {
	for(int i = 0; i < 6; ++i)
	{
	    Coords temp = *it;
	    temp = temp.getNeighbour(i);
	    
	    if(!isControlled(temp)) result.insert(temp);
	}
    }

    return result;
}

std::set<Coords> Growth::tick(std::set<Coords> empty, std::set<Coords> dead, std::set<Coords> enemy)
{
    std::set<Coords> result;
    int duplications = 0;
    
    m_size = m_territory.size();

    m_breathingLength = 0;
    for(auto it = empty.begin(); it != empty.end(); ++it)
    {
	for(int i = 0; i < 6; ++i)
	{
	    if(isControlled(it->getNeighbour(i))) ++m_breathingLength;
	}
    }
    for(auto it = dead.begin(); it != dead.end(); ++it)
    {
	for(int i = 0; i < 6; ++i)
	{
	    if(isControlled(it->getNeighbour(i))) ++m_breathingLength;
	}
    }
    duplications = std::max((int)std::floor(((float)m_breathingLength)/m_gSetts->airPerSynthesis), m_size);
    
    m_fluorescence = ((float)duplications)/((float)m_size);

    if(duplications > enemy.size())
    {
	result = enemy;
	duplications -= enemy.size();
    }
    else
    {
	result = randomlyPick(enemy, duplications);
	duplications = 0;
    }

    if(duplications > dead.size())
    {
	std::copy(dead.begin(), dead.end(), std::inserter(result, result.begin()));
    }
    else if(duplications > 0)
    {
	std::set<Coords> chosen = randomlyPick(dead, duplications);
	std::copy(chosen.begin(), chosen.end(), std::inserter(result, result.begin()));
    }

    return result;
}

void Growth::grow(Coords toAdd)
{
    m_territory.insert(toAdd);
}
