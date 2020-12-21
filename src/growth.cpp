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

std::unordered_set<Coords> Growth::checkConnectedness()
{
    std::unordered_set<Coords> result;
    std::vector<Coords> toMark;
    std::unordered_set<Coords> visited;
    //std::vector<std::set<Coords>::iterator> toDelete;

    if(m_shrinked)
    {
	toMark.emplace_back(m_heart);
	visited.insert(m_heart);
	while(toMark.size() > 0)
	{
	    Coords current = toMark[0];
	    toMark.erase(toMark.begin());

	    for(int i = 0; i < 6; ++i)
	    {
		Coords neighbour = current.getNeighbour(i);
		if(isControlled(neighbour) && visited.find(neighbour) == visited.end())
		{
		    toMark.push_back(neighbour);
		    visited.insert(neighbour);
		}
	    }
	}
	std::cout << "AFTER MARKING\n";

	for(auto it = m_territory.begin(); it != m_territory.end();)
	{
	    if(visited.find(*it) == visited.end())
	    {
		m_edge.insert(*it);
		result.insert(*it);
		it = m_territory.erase(it);
	    }
	    else ++it;
	}
	std::cout << "AFTER TERRITORY\n";
    }

    for(auto it = m_edge.begin(); it != m_edge.end();)
    {
	bool toDelete = true;
	for(int i = 0; i < 6; ++i)
	{
	    Coords temp = it->getNeighbour(i);

	    if(isControlled(temp))
	    {
		toDelete = false;
		break;
	    }
	}
	if(toDelete) it = m_edge.erase(it);
	else ++it;
    }
    std::cout << "AFTER EDGE\n";
    m_shrinked = false;

    return result;
}

std::unordered_set<Coords> Growth::tick(std::unordered_set<Coords> empty,
					std::unordered_set<Coords> dead,
					std::unordered_set<Coords> enemy)
{
    std::unordered_set<Coords> result;
    int duplications = 0;

    m_size = m_territory.size();
    m_duplicating.clear();

    // calculating breathing length
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
    m_breathingLength = std::max(1, m_breathingLength);
    
    duplications = std::min((int)std::floor(((float)m_breathingLength)/m_gSetts->airPerSynthesis), m_size);
    
    m_fluorescence = ((float)duplications)/((float)m_size);

    // choosing territorial expansions
    if(duplications >= enemy.size())
    {
	result = enemy;
	duplications -= enemy.size();
    }
    else
    {
	result = randomlyPick(enemy, duplications);
	duplications = 0;
    }

    if(duplications >= dead.size())
    {
	std::copy(dead.begin(), dead.end(), std::inserter(result, result.begin()));
    }
    else if(duplications > 0)
    {
	std::unordered_set<Coords> chosen = randomlyPick(dead, duplications);
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
	m_shrinked = true;
	m_territory.erase(found);
	m_edge.insert(toSubtract);
	return true;
    }

    return false;
}
