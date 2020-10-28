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
	GridShape::Triangle,
	sf::FloatRect(0, 0, 800, 800),
	sf::Vector2f(10, 10),
	4,
	30
    };

    shared_ptr<BoardSettings> shr_bSetts = make_shared<BoardSettings>(bSetts);

    sf::RenderWindow window(sf::VideoMode(850, 800), "Fill It");
    window.setFramerateLimit(60);
    
    sf::View actionView(sf::Vector2f(425.f, 400.f), sf::Vector2f(850, 800));
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

			    case sf::Keyboard::Num1:
				board.changeColor(0);
				break;

			    case sf::Keyboard::Num2:
				board.changeColor(1);
				break;

			    case sf::Keyboard::Num3:
				board.changeColor(2);
				break;

			    case sf::Keyboard::Num4:
				board.changeColor(3);
				break;

			    case sf::Keyboard::Num5:
				board.changeColor(4);
				break;

			    case sf::Keyboard::Num6:
				board.changeColor(5);
				break;

			    case sf::Keyboard::Num7:
				board.changeColor(6);
				break;

			    case sf::Keyboard::Num8:
				board.changeColor(7);
				break;

			    case sf::Keyboard::Num9:
				board.changeColor(8);
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
