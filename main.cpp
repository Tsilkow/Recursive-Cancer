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

int main(int argc, char* argv[])
{
    int seed = time(NULL);
    
    switch(argc)
    {
	case 2: seed = abs(atoi(argv[1]));
	case 1: break;
    }
    
    srand(seed);
    cout << "seed = " << seed << "\n";

    GrowthSettings gSetts =
    {
	6.f // airPerSynthesis
    };
    shared_ptr<GrowthSettings> shr_gSetts = make_shared<GrowthSettings>(gSetts);
    
    std::vector<Coords> temp;
    BoardSettings bSetts =
    {
        {100, 100},               // dimensions
	{8, 8},                   // tileSize
        sf::Color(  0,   0,   0), // emptyColor
	sf::Color( 64,  64,  64), // deadColor
	sf::Color(255, 255, 255)  // activeColor
    };
    shared_ptr<BoardSettings> shr_bSetts = make_shared<BoardSettings>(bSetts);

    for(int x = 1; x < shr_bSetts->dimensions.x-1; ++x)
    {
	for(int y = 1; y < shr_bSetts->dimensions.y-1; ++y)
	{
	    temp.emplace_back(x, y);
	}
    }

    SimulationSettings sSetts =
    {
	shr_gSetts, // gSetts
	shr_bSetts, // bSetts
	64,         // colorTotal
	41,         // colorWalk
	16,         // cancerImmunity
	0.002f,     // cancerProbability
	temp        // start
    };
    shared_ptr<SimulationSettings> shr_sSetts = make_shared<SimulationSettings>(sSetts);

    sf::RenderWindow window(sf::VideoMode(804, 800), "Recursive Cancer");
    window.setFramerateLimit(6);
    
    sf::View actionView(sf::Vector2f(402.f, 400.f), sf::Vector2f(804, 800));
    window.setView(actionView);

    Simulation simulation(shr_sSetts);

    enum GameState{Simulation};
    GameState currState = GameState::Simulation;
    bool hasFocus = true;
    int ticksPassed = 0;

    while(window.isOpen())
    {
	cout << ticksPassed << endl;
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

	        if(!simulation.tick()) return 0;

		simulation.draw(window);
		break;
	}

	++ticksPassed;
	
	window.display();
    }
    
    return 0;
}
