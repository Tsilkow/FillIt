#pragma once

#include <iostream>
#include <vector>
#include <memory>
#include <math.h>
#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/Audio.hpp>

#include "commons.hpp"


enum GridShape {Triangle, Square, Hexagon};

struct BoardSettings
{
    GridShape gridShape;
    sf::FloatRect bounds;
    sf::Vector2f dimensions;
    int colorTotal;
    int colorStepTotal;
    std::vector<sf::Vector2i> neighbourhood;
};

struct Vector2iComparator
{
    bool operator()(const sf::Vector2i& a, const sf::Vector2i& b);
};

class Board
{
    private:
    std::shared_ptr<BoardSettings> m_bSetts;
    std::vector< std::vector<int> > m_data;
    std::vector<sf::Vertex> m_representation;
    std::vector< std::vector<sf::Color> > m_gradients;

    std::vector<sf::Vector2i> m_currCluster;
    int m_currColor;
    int m_newColor;
    int m_currStep;
    int m_furthestAway;

    void generate();

    void update();

    bool expand();

    public:
    Board(std::shared_ptr<BoardSettings> bSetts);

    bool changeColor(int newColor);
    
    bool tick(int ticksPassed);

    void draw(sf::RenderTarget& target);
};
