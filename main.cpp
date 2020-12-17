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


using namespace std;

int main()
{
    srand(time(NULL));

    BoardSettings bSetts =
    {
        {50, 50},               // dimensions
	{16, 16},                   // tileSize
        sf::Color(  0,   0,   0), // emptyColor
	sf::Color( 64,  64,  64), // deadColor
	sf::Color(255, 255, 255)  // activeColor
    };
    shared_ptr<BoardSettings> shr_bSetts = make_shared<BoardSettings>(bSetts);

    sf::RenderWindow window(sf::VideoMode(808, 808), "Recursive Cancer");
    window.setFramerateLimit(60);
    
    sf::View actionView(sf::Vector2f(404.f, 404.f), sf::Vector2f(808, 808));
    window.setView(actionView);

    Board board(shr_bSetts);

    board.addColor(1, sf::Color(255, 0, 0));
    board.addColor(3, sf::Color(255, 160, 0));
    Coords head(0, 0);
    board.change(head, 3);
    for(int q = 0; q < shr_bSetts->dimensions.y-1; ++q)
    {
	head = head.getNeighbour(2);
	board.change(head, 2*(q % 2)+1);
    }
    for(int q = 0; q < shr_bSetts->dimensions.y-1; ++q)
    {
	head = head.getNeighbour(0);
	board.change(head, -2*(q % 2)+3);
    }

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
		
		board.tick();

		board.draw(window);
		break;
	}

	++ticksPassed;
	
	window.display();
    }
    
    return 0;
}
