#include "board.hpp"


Board::Board(std::shared_ptr<BoardSettings>& bSetts):
    m_bSetts(bSetts)
{
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
		m_depiction.emplace_back(curr[i], m_bSetts->emptyColor);
	    }
	}
    }
}

void Board::update()
{
    for(int i = 0; i < m_toRecolorNow.size(); ++i)
    {
	int index = (m_toRecolorNow[i].first.x * m_bSetts->dimensions.y + m_toRecolorNow[i].first.y)*4;
	sf::Color color;
	if     (m_toRecolorNow[i].second == -2) color = m_bSetts->emptyColor;
	else if(m_toRecolorNow[i].second == -1) color = m_bSetts->deadColor;
	else if(m_toRecolorNow[i].second >=  0) color = m_growthColors[m_toRecolorNow[i].second];
	
	for(int j = 0; j < 4; ++j)
	{
	    m_depiction[index + j].color = color;
	}
    }
    
    m_toRecolorNow = m_toRecolorNext;
	
    for(int i = 0; i < m_toRecolorNext.size(); ++i)
    {
	int index = (m_toRecolorNext[i].first.x * m_bSetts->dimensions.y + m_toRecolorNext[i].first.y)*4;
	for(int j = 0; j < 4; ++j)
	{
	    m_depiction[index + j].color = m_bSetts->activeColor;
	}
    }

    m_toRecolorNext.clear();
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
    if(id >= 0 && m_growthColors.find(id) == m_growthColors.end())
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

    m_toRecolorNext.emplace_back(at, id);
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
