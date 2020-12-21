#include "simulation.hpp"


Simulation::Simulation(std::shared_ptr<SimulationSettings>& sSetts):
    m_sSetts(sSetts),
    m_board(sSetts->bSetts),
    m_nextId(0)
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

void Simulation::newCancer(Coords at)
{
    while(m_growths.find(m_nextId) != m_growths.end())
    {
	m_nextId = (m_nextId+1) % m_sSetts->colorTotal;
    }

    m_growths.insert(std::make_pair(m_nextId, Growth(m_sSetts->gSetts, at)));
    setCell(at, m_nextId);

    ++m_nextId;
}

bool Simulation::tick()
{
    std::unordered_map<Coords, int> expansions;
    
    for(auto it = m_growths.begin(); it != m_growths.end(); ++it)
    {
	std::unordered_set<Coords> edge = it->second.getEdge();
	std::unordered_set<Coords> empty;
	std::unordered_set<Coords> dead;
	std::unordered_set<Coords> enemy;
	std::unordered_set<Coords> plan;

	for(auto jt = edge.begin(); jt != edge.end(); ++jt)
	{
	    if(inBounds(*jt, m_sSetts->bSetts->dimensions.x, m_sSetts->bSetts->dimensions.y))
	    {
		int value = atCoords(m_control, *jt);

		if      (value == -2) empty.insert(*jt);
		else if (value == -1)  dead.insert(*jt);
		else if (value >=  0) enemy.insert(*jt);
	    }
	}
	plan = it->second.tick(empty, dead, enemy);

	for(auto jt = plan.begin(); jt != plan.end(); ++jt)
	{
	    expansions.insert(std::make_pair(*jt, it->first));
	}
    }

    for(auto it = expansions.begin(); it != expansions.end(); ++it)
    {
	if(atCoords(m_control, it->first) < 0 ||
	   !m_growths.find(atCoords(m_control, it->first))->second.isDuplicating(it->first))
	{
	    if(atCoords(m_control, it->first) >= 0)
	    {
		if(!m_growths.find(atCoords(m_control, it->first))->second.shrink(it->first))
		{
		    std::cout << "!ERROR! Simulation representation does not match the on in Growth!\n";
		}
	    }
	    m_growths.find(it->second)->second.grow(it->first);
	    setCell(it->first, it->second);
	}
    }
    
    for(auto it = m_growths.begin(); it != m_growths.end(); ++it)
    {
	std::unordered_set<Coords> temp = it->second.checkConnectedness();

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
