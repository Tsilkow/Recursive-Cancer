#include "growth.hpp"


Growth::Growth(std::shared_ptr<GrowthSettings>& gSetts, Coords heart):
    m_gSetts(gSetts),
    m_heart(heart),
    m_size(1),
    m_breathingLength(0),
    m_fluorescence(1.f)
{
    grow(heart);
}

bool Growth::isControlled(const Coords at)
{
    return (m_territory.find(at) != m_territory.end());
}

bool Growth::isDuplicating(const Coords at)
{
    return (m_duplicating.find(at) != m_duplicating.end());
}

std::set<Coords> Growth::tick(std::set<Coords> empty, std::set<Coords> dead, std::set<Coords> enemy)
{
    std::set<Coords> result;
    int duplications = 0;
    
    m_size = m_territory.size();
    m_duplicating.clear();

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
    duplications = std::min((int)std::floor(((float)m_breathingLength)/m_gSetts->airPerSynthesis), m_size);
    
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

    for(auto it = result.begin(); it != result.end(); ++it)
    {
	for(int i = 0; i < 6; ++i)
	{
	    if(isControlled(it->getNeighbour(i))) m_duplicating.insert(it->getNeighbour(i));
	}
    }

    return result;
}

void Growth::grow(Coords toAdd)
{
    m_territory.insert(toAdd);
    if(m_edge.find(toAdd) != m_edge.end()) m_edge.erase(toAdd);
    else std::cout << "This should happened ONLY with the heart\n";
    
    for(int i = 0; i < 6; ++i)
    {
	Coords temp = toAdd.getNeighbour(i);

	if(!isControlled(temp)) m_edge.insert(temp);
    }
}

bool Growth::shrink(Coords toSubtract)
{
    auto found = m_territory.find(toSubtract);

    if(found != m_territory.end())
    {
	bool isEdge = false;
	m_territory.erase(found);

	// add subtracted terrirtory to edge ONLY if it still has a neighbour from the same organism
	for(int i = 0; i < 6; ++i)
	{
	    Coords temp = toSubtract.getNeighbour(i);
	    
	    if(isControlled(temp))
	    {
		isEdge = true;
		break;
	    }
	}
	if(isEdge) m_edge.insert(toSubtract);

	// erase all coordinates considered as edge (so adjacent to the growth)
	//  ONLY because of now subtracted territory
	for(int i = 0; i < 6; ++i)
	{
	    isEdge = false;
	    Coords temp = toSubtract.getNeighbour(i);
	    
	    if(!isControlled(temp))
	    {
		for(int j = 0; j < 6; ++j)
		{
		    Coords temp2 = temp.getNeighbour(j);
		    if(isControlled(temp2))
		    {
			isEdge = true;
			break;
		    }
		}
		if(!isEdge) m_edge.erase(temp);
	    }
	}
	
	return true;
    }

    return false;
}
