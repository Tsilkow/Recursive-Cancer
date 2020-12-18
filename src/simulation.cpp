#include "simulation.hpp"


Simulation::Simulation(std::shared_ptr<SimulationSettings>& sSetts):
    m_sSetts(sSetts),
    m_board(sSetts->bSetts)
{
    std::vector<sf::Color> palette = generatePalette(sSetts->colorTotal);
    for(int i = 0; i < palette.size(); ++i)
    {
	m_board.addColor(i, palette[i]);
    }
}

bool Simulation::tick()
{
    m_board.tick();

    return true;
}

void Simulation::draw(sf::RenderTarget& target)
{
    m_board.draw(target);
}
