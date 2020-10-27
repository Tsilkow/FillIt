#include "board.hpp"


bool Vector2iComparator::operator()(const sf::Vector2i& a, const sf::Vector2i& b)
{
    if(a.x < b.x) return true;
    if(a.y < b.y) return true;
    return false;
}

Board::Board(std::shared_ptr<BoardSettings> bSetts):
    m_bSetts(bSetts)
{
    int tileSize = std::min((int)std::round(m_bSetts->bounds.width / (float)m_bSetts->dimensions.x),
			    (int)std::round(m_bSetts->bounds.width / (float)m_bSetts->dimensions.x));

    generate();

    std::vector<sf::Color> palette = generatePalette(m_bSetts->colorsTotal);

    m_gradients = std::vector< std::vector<sf::Color> >(m_bSetts->colorsTotal, std::vector<sf::Color>());
    for(int i = 0; i < m_bSetts->colorsTotal; ++i)
    {
	m_gradients[i] = colorGradient(palette[i], sf::Color(255, 255, 255), m_bSetts->colorStepTotal);
    }
    
    for(int x = 0; x < m_bSetts->dimensions.x; ++x)
    {
	for(int y = 0; y < m_bSetts->dimensions.x; ++y)
	{
	    std::vector<sf::Vector2f> position = {
		sf::Vector2f( x   *tileSize,  y   *tileSize),
		sf::Vector2f((x+1)*tileSize,  y   *tileSize),
		sf::Vector2f((x+1)*tileSize, (y+1)*tileSize),
		sf::Vector2f( x   *tileSize, (y+1)*tileSize)
	    };

	    for(int i = 0; i < 4; ++i)
	    {
		m_representation.emplace_back(position[i] + 
					      sf::Vector2f(m_bSetts->bounds.left, m_bSetts->bounds.top),
					      m_gradients[atCoords(m_data, sf::Vector2i(x, y))-1][0]);
	    }
	}
    }
}

void Board::generate()
{
    m_data = std::vector< std::vector<int> >(m_bSetts->dimensions.x,
					     std::vector<int>(m_bSetts->dimensions.y, 0));
    for(int x = 0; x < m_bSetts->dimensions.x; ++x)
    {
	for(int y = 0; y < m_bSetts->dimensions.x; ++y)
	{
	    m_data[x][y] = randomI(1, m_bSetts->colorsTotal);
	}
    }

    m_currCluster.emplace_back(0, 0);
    m_currColor = m_data[0][0]-1;
    m_currStep = 0;
}

void Board::update()
{
    for(int i = 0; i < m_currCluster.size(); ++i)
    {
	int index = m_currCluster[i].y * m_bSetts->dimensions.x + m_currCluster[i].x;
	int verticeTotal = 0;

	switch(m_bSetts->gridShape)
	{
	    case Triangle: verticeTotal = 3; break;
	    case Square: verticeTotal = 4; break;
	    case Hexagon: verticeTotal = 6; break;
	}
	index *= verticeTotal;

	for(int j = 0; j < verticeTotal; ++j)
	{
	    if(m_currStep < m_bSetts->colorStepTotal)
	    {
		m_representation[index + j].color = m_gradients[m_currColor][m_currStep];
	    }
	    else
	    {
		m_representation[index + j].color = m_gradients[m_newColor ][m_currStep];
	    }
	    
	}
    }
}

bool Board::expand()
{
    std::vector<sf::Vector2i> queue = m_currCluster;
    std::set<sf::Vector2i, Vector2iComparator> visited;

    for(int i = 0; i < queue.size(); ++i)
    {
	visited.insert(queue[i]);
    }

    while(queue.size() > 0)
    {
	sf::Vector2i current = queue[0];

	for(int i = 0; i < m_bSetts->neighbourhood.size(); ++i)
	{
	    sf::Vector2i neighbour = current + m_bSetts->neighbourhood[i];
	    
	    if(neighbour.x >= 0 && neighbour.x <= m_bSetts->dimensions.x &&
	       neighbour.y >= 0 && neighbour.y <= m_bSetts->dimensions.y &&
	       visited.find(current + m_bSetts->neighbourhood[i]) == visited.end() &&
	       atCoords(m_data, neighbour) == m_newColor)
	    {
		m_currCluster.emplace_back(neighbour);
		queue.push_back(neighbour);
		visited.insert(neighbour);
	    }
	}
    }

    return true;
}

bool Board::changeColor(int newColor)
{
    if(m_currStep == 0)
    {
	m_newColor = newColor;
	m_currStep = 1;
	return true;
    }
    
    return false;
}

bool Board::tick(int ticksPassed)
{
    if(m_currStep != 0)
    {
	update();
        ++m_currStep;
	if(m_currStep >= m_bSetts->colorStepTotal*2)
	{
	    m_currStep = 0;
	    expand();
	    m_currColor = m_newColor;

	    for(int i = 0; i < m_currCluster.size(); ++i)
	    {
		atCoords(m_data, m_currCluster[i]) = m_currColor;
	    }
	}
    }

    return false;
}

void Board::draw(sf::RenderTarget& target)
{
    target.draw(&m_representation[0], m_representation.size(), sf::Quads);
}
