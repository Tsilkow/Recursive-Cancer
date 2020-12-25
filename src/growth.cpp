#include "growth.hpp"


Growth::Growth(std::shared_ptr<GrowthSettings>& gSetts, Coords heart):
    m_gSetts(gSetts),
    m_heart(heart),
    m_size(1),
    m_breathingLength(0),
    m_fluorescence(1.f),
    m_host(nullptr)
{
    m_edge.insert(m_heart);
    
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
	if(!isControlled(m_heart))
	{
	    result = m_territory;
	    m_territory.clear();
	}
	else
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
	    //std::cout << "AFTER MARKING\n";

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
	}
	//std::cout << "AFTER TERRITORY\n";
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
    //std::cout << "AFTER EDGE\n";
    m_shrinked = false;

    return result;
}

void Growth::breathe(std::unordered_set<Coords> empty,
		     std::unordered_set<Coords> dead,
		     std::unordered_set<Coords> enemy)
{
    m_empty = empty;
    m_dead = dead;
    m_enemy = enemy;

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
    
    int duplications =
	std::min(m_size, (int)std::floor(((float)m_breathingLength)/m_gSetts->airPerSynthesis));

    if(m_host != nullptr)
    {
        m_host->request(m_heart, std::min(m_size - duplications,
					  (int)(dead.size() + enemy.size() - duplications)));
    }
}

void Growth::feed()
{
    int total = 0;
    int left = 0;
    int duplications =
	std::min(m_size, (int)std::floor(((float)m_breathingLength)/m_gSetts->airPerSynthesis));
    
    for(auto it = m_requests.begin(); it != m_requests.end(); ++it)
    {
	total += it->second;
    }

    if(duplications < total)
    {
	float ratio = ((float)duplications)/((float)total);
	std::map<float, Coords> remainder;
	left = duplications;

	for(auto it = m_requests.begin(); it != m_requests.end(); ++it)
	{
	    remainder.insert(std::make_pair(it->second * ratio - std::floor(it->second * ratio), it->first));
	    it->second = std::floor(it->second * ratio);
	    left -= it->second;
	}

	for(auto it = remainder.begin(); left > 0 && it != remainder.end(); ++it)
	{
	    ++m_requests.find(it->second)->second;
	    --left;
	}
	m_breathingLength = 0;
    }
    else m_breathingLength -= total * m_gSetts->airPerSynthesis;
}

std::unordered_set<Coords> Growth::getPlan()
{
    std::unordered_set<Coords> result;
    
    int duplications =
	std::min((int)std::floor(((float)m_breathingLength)/m_gSetts->airPerSynthesis), m_size);
    m_fluorescence = ((float)duplications)/((float)m_size);

    if(m_host != nullptr) duplications += m_host->getRequested(m_heart);

    //std::cout << duplications << std::endl;

    // choosing territorial expansions
    if(duplications >= m_enemy.size())
    {
	result = m_enemy;
	duplications -= m_enemy.size();
    }
    else
    {
	result = randomlyPick(m_enemy, duplications);
	duplications = 0;
    }

    if(duplications >= m_dead.size())
    {
	std::copy(m_dead.begin(), m_dead.end(), std::inserter(result, result.begin()));
    }
    else if(duplications > 0)
    {
	std::unordered_set<Coords> chosen = randomlyPick(m_dead, duplications);
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
    else std::cout << "Error! Added a cell that was not considered to be on the edge.\n";
    
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

void Growth::request(Coords from, int amount)
{
    if(amount != 0) m_requests.insert(std::make_pair(from, amount));
}

int Growth::getRequested(Coords from)
{
    int result = 0;
    auto found = m_requests.find(from);
    
    if(found != m_requests.end())
    {
	result = found->second;
	m_requests.erase(found);
    }
    
    return result;
}

void Growth::setHost(std::shared_ptr<Growth>& host)
{
    m_host = host;
}

void Growth::resetHost()
{
    m_host = nullptr;
}
