#include "board.hpp"


bool Vector2iComparator::operator()(const sf::Vector2i& a, const sf::Vector2i& b)
{
    if(a.x < b.x) return true;
    if(a.x == b.x && a.y < b.y) return true;
    return false;
}

Board::Board(std::shared_ptr<BoardSettings> bSetts):
    m_bSetts(bSetts),
    m_furthestAway(0)
{
    switch(m_bSetts->tileShape)
    {
	case Triangle: m_verticeTotal = 3; break;
	case Square:   m_verticeTotal = 4; break;
	case Hexagon:  m_verticeTotal = 6; break;
    }
    
    generate();

    std::vector<sf::Color> palette = generatePalette(m_bSetts->colorTotal);

    m_gradients = std::vector< std::vector<sf::Color> >(m_bSetts->colorTotal, std::vector<sf::Color>());
    for(int i = 0; i < m_bSetts->colorTotal; ++i)
    {
	m_gradients[i] = colorGradient(palette[i], sf::Color(255, 255, 255), m_bSetts->colorStepTotal);
	/*for(int j = 0; j < m_gradients[i].size(); ++j)
	{
	    std::cout << i << "|" << j << " = ("
		      << (int)m_gradients[i][j].r << ", "
		      << (int)m_gradients[i][j].g << ", "
		      << (int)m_gradients[i][j].b << ")" << std::endl;
		      }*/
    }

    int tileSize;
    switch(m_bSetts->tileShape)
    {
	case Triangle:
	    tileSize = std::min((int)std::round(2.f * m_bSetts->bounds.width /
						((float)m_bSetts->dimensions.x + 0.5f)),
				(int)std::round(m_bSetts->bounds.height / sqrt(3.f) * 2.f /
						(float)m_bSetts->dimensions.y));
	    break;
	case Square:   
	    tileSize = std::min((int)std::round(m_bSetts->bounds.width  / (float)m_bSetts->dimensions.x),
				(int)std::round(m_bSetts->bounds.height / (float)m_bSetts->dimensions.y));
	    break;
	case Hexagon:
	    tileSize = std::min((int)std::round(2.f * m_bSetts->bounds.width /
						(3.f * m_bSetts->dimensions.x + 1.0f)),
				(int)std::round(m_bSetts->bounds.height / sqrt(3.f) /
						((float)m_bSetts->dimensions.y + 0.5f)));
	    break;
    }
    
    for(int x = 0; x < m_bSetts->dimensions.x; ++x)
    {
	for(int y = 0; y < m_bSetts->dimensions.y; ++y)
	{
	    std::vector<sf::Vector2f> position;
	    switch(m_bSetts->tileShape)
	    {
		case Triangle:
		    if(modulo(x+y, 2) == 0)
		    {
			position.emplace_back((x + 1) * 0.5f * tileSize,  y    * sqrt(3.f)/2.f * tileSize);
			position.emplace_back((x + 2) * 0.5f * tileSize, (y+1) * sqrt(3.f)/2.f * tileSize);
			position.emplace_back( x      * 0.5f  * tileSize, (y+1) * sqrt(3.f)/2.f * tileSize);
		    }
		    else
		    {
			position.emplace_back( x      * 0.5f * tileSize,  y    * sqrt(3.f)/2.f * tileSize);
			position.emplace_back((x + 2) * 0.5f * tileSize,  y    * sqrt(3.f)/2.f * tileSize);
			position.emplace_back((x + 1) * 0.5f * tileSize, (y+1) * sqrt(3.f)/2.f * tileSize);
		    }
		    break;
		    
		case Square:
		    position.emplace_back( x    * tileSize,  y    * tileSize);
		    position.emplace_back((x+1) * tileSize,  y    * tileSize);
		    position.emplace_back((x+1) * tileSize, (y+1) * tileSize);
		    position.emplace_back( x    * tileSize, (y+1) * tileSize);
		    break;
		    
		case Hexagon:
		    position.emplace_back( x    * 1.5f         * tileSize,
					 ( y    + 0.5f * (1 + modulo(x, 2))) * sqrt(3.f) * tileSize);
		    position.emplace_back((x    * 1.5f + 0.5f) * tileSize,
					 ( y    + 0.5f *      modulo(x, 2))  * sqrt(3.f) * tileSize);
		    position.emplace_back((x    * 1.5f + 1.5f) * tileSize,
					 ( y    + 0.5f *      modulo(x, 2))  * sqrt(3.f) * tileSize);
		    position.emplace_back((x    * 1.5f + 2.f ) * tileSize,
					 ( y    + 0.5f * (1 + modulo(x, 2))) * sqrt(3.f) * tileSize);
		    position.emplace_back((x    * 1.5f + 1.5f) * tileSize,
					 ((y+1) + 0.5f *      modulo(x, 2))  * sqrt(3.f) * tileSize);
		    position.emplace_back((x    * 1.5f + 0.5f) * tileSize,
					 ((y+1) + 0.5f *      modulo(x, 2))  * sqrt(3.f) * tileSize);
		    break;
	    }

	    for(int i = 0; i < m_verticeTotal; ++i)
	    {
		m_representation.emplace_back(position[i] + 
					      sf::Vector2f(m_bSetts->bounds.left, m_bSetts->bounds.top),
					      m_gradients[atCoords(m_data, sf::Vector2i(x, y))-1][0]);
	    }
	}
    }
}

sf::Vector2i Board::getNeighbour(sf::Vector2i coords, int direction)
{
    sf::Vector2i result = coords;
    
    switch(m_bSetts->tileShape)
    {
	case Triangle:
	    if(modulo(coords.x + coords.y, 2) == 0)
	    {
		// top pointed triangle
		switch(direction)
		{
		    case 0: ++result.x; break;
		    case 1: ++result.y; break;
		    case 2: --result.x; break;
		}
	    }
	    else
	    {
		// bottom pointed triangle
		switch(direction)
		{
		    case 0: --result.y; break;
		    case 1: ++result.x; break;
		    case 2: --result.x; break;
		}
	    }
	    break;
	case Square:
	    switch(direction)
	    {
		case 0: --result.y; break;
		case 1: ++result.x; break;
		case 2: ++result.y; break;
		case 3: --result.x; break;
	    }
	    break;
	case Hexagon:
	    if(modulo(coords.x, 2) == 0)
	    {
		// "higher" collum
		switch(direction)
		{
		    case 0: --result.y;             break;
		    case 1: ++result.x; --result.y; break;
		    case 2: ++result.x;             break;
		    case 3: ++result.y;             break;
		    case 4: --result.x;             break;
		    case 5: --result.x; ++result.y; break;
		}
	    }
	    else 
	    {
		// "lower" collum
		switch(direction)
		{
		    case 0: --result.y;             break;
		    case 1: ++result.x;             break;
		    case 2: ++result.x; ++result.y; break;
		    case 3: ++result.y;             break;
		    case 4: --result.x; ++result.y; break;
		    case 5: --result.x;             break;
		}
	    }
	    break;
    }

    return result;
}

void Board::generate()
{
    m_data = std::vector< std::vector<int> >(m_bSetts->dimensions.x,
					     std::vector<int>(m_bSetts->dimensions.y, 0));
    for(int x = 0; x < m_bSetts->dimensions.x; ++x)
    {
	for(int y = 0; y < m_bSetts->dimensions.y; ++y)
	{
	    m_data[x][y] = randomI(1, m_bSetts->colorTotal);
	}
    }

    m_currCluster.emplace_back(0, 0);
    m_currColor = m_data[0][0]-1;
    m_newColor = m_currColor;
    m_currStep = 0;

    expand();
}

void Board::update()
{
    for(int i = 0; i < m_currCluster.size(); ++i)
    {
	int index = m_verticeTotal * (m_currCluster[i].x * m_bSetts->dimensions.y + m_currCluster[i].y);
	// perceived step
	int percStep = m_currStep - (m_currCluster[i].x + m_currCluster[i].y);
	m_furthestAway = std::max(m_furthestAway, m_currCluster[i].x + m_currCluster[i].y);
	
	if(percStep < 0 || percStep >= m_bSetts->colorStepTotal*2);
	else if(percStep < m_bSetts->colorStepTotal)
	{
	    for(int j = 0; j < m_verticeTotal; ++j)
	    {
		m_representation[index + j].color =
		    m_gradients[m_currColor][percStep];
	    }
	}
	else
	{
	    for(int j = 0; j < m_verticeTotal; ++j)
	    {
		m_representation[index + j].color =
		    m_gradients[m_newColor ][2*m_bSetts->colorStepTotal-1 - percStep];
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
	queue.erase(queue.begin());

	for(int i = 0; i < m_verticeTotal; ++i)
	{
	    sf::Vector2i neighbour = getNeighbour(current, i);
	    
	    if(neighbour.x >= 0 && neighbour.x < m_bSetts->dimensions.x &&
	       neighbour.y >= 0 && neighbour.y < m_bSetts->dimensions.y &&
	       visited.find(neighbour) == visited.end() &&
	       atCoords(m_data, neighbour) == m_newColor+1)
	    {
		m_currCluster.emplace_back(neighbour);
		//std::cout << "EXPANDED: ";
		//printVector(neighbour, true);
		queue.push_back(neighbour);
		visited.insert(neighbour);
	    }
	}
    }

    return true;
}

bool Board::changeColor(int newColor)
{
    if(newColor != m_currColor && m_currStep == 0 && newColor < m_bSetts->colorTotal)
    {
	m_newColor = newColor;
	m_currStep = 1;
	return true;
    }
    
    return false;
}

bool Board::tick(int ticksPassed)
{
    //std::cout << m_currStep << std::endl;
    
    if(m_currStep != 0)
    {
	update();
        ++m_currStep;
	if(m_currStep >= m_bSetts->colorStepTotal*2 + m_furthestAway)
	{
	    m_currStep = 0;
	    expand();
	    m_currColor = m_newColor;

	    for(int i = 0; i < m_currCluster.size(); ++i)
	    {
		atCoords(m_data, m_currCluster[i]) = m_currColor+1;
	    }
	}
    }

    return false;
}

void Board::draw(sf::RenderTarget& target)
{
    for(int i = 0; i < m_representation.size(); i += m_verticeTotal)
    {
	target.draw(&m_representation[i], m_verticeTotal, sf::TriangleFan);
    }
}
