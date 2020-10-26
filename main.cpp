// Tsilkow

#include <iostream>
#include <vector>
#include <time.h>
#include <math.h>
#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/Audio.hpp>

#include "commons.hpp"


using namespace std;

int main()
{
    srand(time(NULL));

    sf::RenderWindow window(sf::VideoMode(800, 800), "Clear Asteroids");
    window.setFramerateLimit(60);
    
    sf::View actionView(sf::Vector2f(0.f, 0.f), sf::Vector2f(800, 800));
    window.setView(actionView);

    enum GameState{Menu, Play, Scores};
    GameState currState = GameState::Menu;
    bool hasFocus = true;
    int ticksPassed = 0;

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
