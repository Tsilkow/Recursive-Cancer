#include "simulation.hpp"


Simulation::Simulation(std::shared_ptr<SimulationSettings>& sSetts):
    m_sSetts(sSetts),
    m_board(sSetts->bSetts),
    m_nextId(randomI(0, m_sSetts->colorTotal-1))
{
    std::vector<sf::Color> palette = generatePalette(sSetts->colorTotal);
    for(int i = 0; i < palette.size(); ++i)
    {
	m_board.addColor(i, palette[i]);
    }

    m_control = std::vector< std::vector<int> >(m_sSetts->bSetts->dimensions.x,
						std::vector<int>(m_sSetts->bSetts->dimensions.y, -2));

    for(int i = 0; i < m_sSetts->start.size(); ++i)
    {
	setCell(m_sSetts->start[i], -1);
    }

    newCancer(m_sSetts->start[randomI(0, m_sSetts->start.size()-1)]);
    newCancer(m_sSetts->start[randomI(0, m_sSetts->start.size()-1)]);

    m_board.tick();
}

void Simulation::setCell(Coords at, int to)
{
    atCoords(m_control, at) = to;
    m_board.change(at, to);
}

void Simulation::newCancer(Coords at, int mutatedFrom)
{
    while(m_growths.find(m_nextId) != m_growths.end())
    {
	m_nextId = (m_nextId + m_sSetts->colorWalk) % m_sSetts->colorTotal;
    }

    m_growths.insert(std::make_pair(m_nextId, std::make_shared<Growth>(Growth(m_sSetts->gSetts, at))));
    if(mutatedFrom != -1)
    {
	m_cancerImmunity.insert(
	       std::make_pair(mutatedFrom, std::make_pair(m_nextId, m_sSetts->cancerImmunity)));
	m_growths[m_nextId]->setHost(m_growths[mutatedFrom]);
    }
    setCell(at, m_nextId);
}

bool Simulation::tick()
{
    std::multimap<Coords, int> expansions;
    
    for(auto it = m_growths.begin(); it != m_growths.end();)
    {
	std::unordered_set<Coords> edge = it->second->getEdge();
	std::unordered_set<Coords> empty;
	std::unordered_set<Coords> dead;
	std::unordered_set<Coords> enemy;

	std::unordered_set<int> unrecognizedCancers;
	std::pair<std::unordered_multimap<int, std::pair<int, int>>::iterator,
		  std::unordered_multimap<int, std::pair<int, int>>::iterator> pointers =
	    m_cancerImmunity.equal_range(it->first);

	for(auto jt = pointers.first; jt != pointers.second; ++jt)
	{
	    auto found = m_growths.find(jt->second.first);

	    if(found != m_growths.end()) unrecognizedCancers.insert(jt->second.first);
	}

	for(auto jt = edge.begin(); jt != edge.end(); ++jt)
	{
	    if(inBounds(*jt, m_sSetts->bSetts->dimensions.x, m_sSetts->bSetts->dimensions.y))
	    {
		int value = atCoords(m_control, *jt);

		if      (value == -2 ||
			 unrecognizedCancers.find(value) != unrecognizedCancers.end())
			 empty.insert(*jt);
		else if (value == -1)  dead.insert(*jt);
		else if (value >=  0) enemy.insert(*jt);
	    }
	}

	it->second->breathe(empty, dead, enemy);
	if(it->second->getSize() == 0)
	{
	    it = m_growths.erase(it);

	    for(auto jt = unrecognizedCancers.begin(); jt != unrecognizedCancers.end(); ++jt)
	    {
		m_growths[*jt]->resetHost();
	    }
	}
	else ++it;
    }

    //std::cout << "AFTER BREATHING" << std::endl;

    for(auto it = m_growths.begin(); it != m_growths.end(); ++it)
    {
	it->second->feed();
    }

    //std::cout << "AFTER FEEDING" << std::endl;
    
    for(auto it = m_growths.begin(); it != m_growths.end(); ++it)
    {
	std::unordered_set<Coords> plan = it->second->getPlan();

	for(auto jt = plan.begin(); jt != plan.end(); ++jt)
	{
	    expansions.insert(std::make_pair(*jt, it->first));
	}
    }
    
    //std::cout << "AFTER PLANNING" << std::endl;

    for(auto it = m_cancerImmunity.begin(); it != m_cancerImmunity.end();)
    {
	auto found = m_growths.find(it->second.first);
	    
	--it->second.second;
	if(found == m_growths.end()) it = m_cancerImmunity.erase(it);
	else if(it->second.second <= 0)
	{
	    if(found != m_growths.end()) m_growths[it->second.first]->resetHost();
	    it = m_cancerImmunity.erase(it);
	}
	else ++it;
    }

    //std::cout << "AFTER CANCER AWARENESS" << std::endl;
    
    for(auto it = expansions.begin(); it != expansions.end();)
    {
	std::pair<std::map<Coords, int>::iterator, std::map<Coords, int>::iterator>
	    range = expansions.equal_range(it->first);
	std::size_t distance = std::distance(range.first, range.second);
	it = range.first;
	std::advance(it, randomI(0, distance-1));
	
	if(atCoords(m_control, it->first) < 0 ||
	   !m_growths.find(atCoords(m_control, it->first))->second->isDuplicating(it->first))
	{
	    
	    if(atCoords(m_control, it->first) >= 0)
	    {
		if(!m_growths.find(atCoords(m_control, it->first))->second->shrink(it->first))
		{
		    std::cout << "!ERROR! Simulation representation does not match the on in Growth!\n";
		}
	    }
	    if(randomF(0.f, 1.f, 0.001f) < m_sSetts->cancerProbability) newCancer(it->first, it->second);
	    else
	    {
		m_growths.find(it->second)->second->grow(it->first);
		setCell(it->first, it->second);
	    }
	}

	it = range.second;
    }
    
    //std::cout << "AFTER EXPANDING" << std::endl;
    
    for(auto it = m_growths.begin(); it != m_growths.end(); ++it)
    {
	std::unordered_set<Coords> temp = it->second->checkConnectedness();

	for(auto jt = temp.begin(); jt != temp.end(); ++jt)
	{
	    if(atCoords(m_control, *jt) != it->first) std::cout << "DUDE, DON'T DELETE SOMETHING THAT'S NOT YOURS\n";
	    setCell(*jt, -1);
	}
    }
    
    m_board.tick();

    if(expansions.size() == 0) return false;
    
    return true;
}

void Simulation::draw(sf::RenderTarget& target)
{
    m_board.draw(target);
}
