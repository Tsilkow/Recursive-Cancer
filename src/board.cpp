#include "board.hpp"


Board::Board(std::shared_ptr<BoardSettings>& bSetts):
    m_bSetts(bSetts)
{
    m_data = std::vector< std::vector<int> >(m_bSetts->dimensions.x,
					     std::vector<int>(m_bSetts->dimensions.y, -2));

    for(int i = 0; i < m_bSetts->start.size(); ++i)
    {
	atCoords(m_data, m_bSetts->start[i]) = -1;
    }

    for(int x = 0; x < m_bSetts->dimensions.x; ++x)
    {
	for(int y = 0; y < m_bSetts->dimensions.y; ++y)
	{
	    std::vector<sf::Vector2f> curr;
	    curr.emplace_back((x + (y%2) * 0.5  ) * m_bSetts->tileSize.x,  y    * m_bSetts->tileSize.y);
	    curr.emplace_back((x + (y%2) * 0.5+1) * m_bSetts->tileSize.x,  y    * m_bSetts->tileSize.y);
	    curr.emplace_back((x + (y%2) * 0.5+1) * m_bSetts->tileSize.x, (y+1) * m_bSetts->tileSize.y);
	    curr.emplace_back((x + (y%2) * 0.5  ) * m_bSetts->tileSize.x, (y+1) * m_bSetts->tileSize.y);

	    for(int i = 0; i < 4; ++i)
	    {
		if(m_data[x][y] == -2) m_depiction.emplace_back(curr[i], m_bSetts->emptyColor);
		else m_depiction.emplace_back(curr[i], m_bSetts->deadColor);
	    }
	}
    }
}

void Board::update()
{
    for(int i = 0; i < m_toRecolor.size(); ++i)
    {
	int index = (m_toRecolor[i].x * m_bSetts->dimensions.y + m_toRecolor[i].y)*4;
	for(int j = 0; j < 4; ++j)
	{
	    m_depiction[index + j].color = m_growthColors[atCoords(m_data, m_toRecolor[i])];
	}
    }
    
    m_toRecolor = m_toUpdate;
	
    for(int i = 0; i < m_toUpdate.size(); ++i)
    {
	int index = (m_toUpdate[i].x * m_bSetts->dimensions.y + m_toUpdate[i].y)*4;
	for(int j = 0; j < 4; ++j)
	{
	    m_depiction[index + j].color = m_bSetts->activeColor;
	}
    }

    m_toUpdate.clear();
}

bool Board::addColor(int id, sf::Color color)
{
    bool duplicate = false;
    for(auto it = m_growthColors.begin(); it != m_growthColors.end(); ++it)
    {
	if(it->second == color) duplicate = true;
    }
    
    if(duplicate)
    {
	std::cout << "Duplicated color! ";
	printColor(color, false);
	std::cout << " was NOT added."  << std::endl;
	return false;
    }

    if(id < 0)
    {
	std::cout << "Negative id! " << id << " was NOT added."  << std::endl;
	return false;
    }

    m_growthColors[id] = color;
    return true;
}

bool Board::change(Coords at, int id)
{
    if(m_growthColors.find(id) == m_growthColors.end())
    {
	std::cout << "No color assigned for id " << id << "! Data at ";
	at.print(false);
	std::cout << " was not changed." << std::endl;
	return false;
    }
    
    if(!inBounds(at, m_bSetts->dimensions.x, m_bSetts->dimensions.y))
    {
	std::cout << "Coords ";
	at.print(false);
	std::cout << " are out of bounds! Data at those coordinates was not changed.\n";
	return false;
    }

    atCoords(m_data, at) = id;
    m_toUpdate.emplace_back(at);
    return true;
}

void Board::tick()
{
    update();
}

void Board::draw(sf::RenderTarget& target)
{
    target.draw(&m_depiction[0], m_depiction.size(), sf::Quads);
}
