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
	GridShape::Square,
	sf::FloatRect(0, 0, 800, 800),
	sf::Vector2f(10, 10),
	6,
	30,
	{sf::Vector2i( 0, -1),
	 sf::Vector2i( 1,  0),
	 sf::Vector2i( 0,  1),
	 sf::Vector2i(-1,  0)}
    };

    shared_ptr<BoardSettings> shr_bSetts = make_shared<BoardSettings>(bSetts);

    sf::RenderWindow window(sf::VideoMode(800, 800), "Clear Asteroids");
    window.setFramerateLimit(60);
    
    sf::View actionView(sf::Vector2f(0.f, 0.f), sf::Vector2f(800, 800));
    window.setView(actionView);

    enum GameState{Menu, Play, Scores};
    GameState currState = GameState::Play;
    bool hasFocus = true;
    int ticksPassed = 0;

    Board board(shr_bSetts);

    while(window.isOpen())
    {
	sf::Event event;
	std::pair<std::string, std::string> input;
	
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
			}
		    }
		    break;
	    }

	    if(hasFocus)
	    {
		switch(currState)
		{
		    case GameState::Play:
			break;
		}
	    }
	}
	
	switch(currState)
	{	
	    case GameState::Play:
		board.tick(ticksPassed);
		board.draw(window);
		break;
	}

	if(hasFocus)
	{
	    ++ticksPassed;
	}
	
	window.display();
    }
    
    return 0;
}
