// Tsilkow

#include <iostream>
#include <vector>
#include <time.h>
#include <memory>
#include <math.h>
#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/Audio.hpp>

#include "commons.hpp"
#include "board.hpp"
#include "simulation.hpp"


using namespace std;

int main()
{
    srand(time(NULL));

    Coords temp(24, 24);
    BoardSettings bSetts =
    {
        {50, 50},               // dimensions
	{16, 16},                   // tileSize
        sf::Color(  0,   0,   0), // emptyColor
	sf::Color( 64,  64,  64), // deadColor
	sf::Color(255, 255, 255), // activeColor
	{temp.getNeighbour(0),
	 temp.getNeighbour(1),
	 temp.getNeighbour(2),
	 temp.getNeighbour(3),
	 temp.getNeighbour(4),
	 temp.getNeighbour(5)}
    };
    shared_ptr<BoardSettings> shr_bSetts = make_shared<BoardSettings>(bSetts);

    SimulationSettings sSetts =
    {
	shr_bSetts, // bSetts
	16          // colorTotal
    };
    shared_ptr<SimulationSettings> shr_sSetts = make_shared<SimulationSettings>(sSetts);

    sf::RenderWindow window(sf::VideoMode(808, 808), "Recursive Cancer");
    window.setFramerateLimit(60);
    
    sf::View actionView(sf::Vector2f(404.f, 404.f), sf::Vector2f(808, 808));
    window.setView(actionView);

    Simulation simulation(shr_sSetts);

    enum GameState{Simulation};
    GameState currState = GameState::Simulation;
    bool hasFocus = true;
    int ticksPassed = 0;

    while(window.isOpen())
    {
	sf::Event event;
	
	window.clear();
	
	while (window.pollEvent(event))
	{
	    switch(event.type)
	    {
		case sf::Event::Closed:
		    window.close();
		    break;
		case sf::Event::LostFocus:
		    hasFocus = false;
		    //std::cout << "LOST FOCUS" << std::endl;
		    break;
		case sf::Event::GainedFocus:
		    hasFocus = true;
		    //std::cout << "GAINED FOCUS" << std::endl;
		    break;
		case sf::Event::KeyPressed:
		    if(hasFocus)
		    {
			switch(event.key.code)
			{
			    case sf::Keyboard::Escape:
				window.close();
				break;
				
			    default: break;
			}
		    }
		    break;
		    
		default: break;
	    }

	    if(hasFocus)
	    {
		switch(currState)
		{
		    case GameState::Simulation:
			break;
			
		    default: break;
		}
	    }
	}
	
	switch(currState)
	{	
	    case GameState::Simulation:
		
	        simulation.tick();

		simulation.draw(window);
		break;
	}

	++ticksPassed;
	
	window.display();
    }
    
    return 0;
}
