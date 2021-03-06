#include "commons.hpp"


#include <stdlib.h>
#include <iomanip>
#include <sstream>
#include <math.h>


int modulo(int a, int b)
{
    while(a <  0) a += b;
    while(a >= b) a -=b;

    return a;
}

float modulo(float a, float b)
{
    while(a <  0) a += b;
    while(a >= b) a -=b;

    return a;
}

int randomI(int min, int max)
{
    if(min == max) return min;
    return rand() % (max - min + 1) + min;
}

float randomF(float min, float max, float step)
{
    return randomI(std::round(min * 1.f/step), std::round(max * 1.f/step)) * step;
}

float radToDeg(float radians)
{
    return radians * 180/M_PI;
}

float normalizeAngle(float angle)
{
    while(angle <       0) angle += 2*M_PI;
    while(angle >= 2*M_PI) angle -= 2*M_PI;
    return angle;
}

float length(sf::Vector2f input)
{
    return sqrt(input.x*input.x + input.y*input.y);
}

float distance(sf::Vector2f a, sf::Vector2f b)
{
    return length(a - b);
}

float determineLightness(sf::Color input)
{
    // https://thoughtbot.com/blog/closer-look-color-lightness
    // values have been normalized (ie divided by 255)
    return ((float)input.r) * 0.00083372549 +
	   ((float)input.g) * 0.00280470588 +
	   ((float)input.b) * 0.00028313725;
}

sf::Color randomColor(std::vector<float> lightRange)
{
    sf::Color result(0, 0, 0);
    do{
	result.r = randomI(0, 255);
	result.g = randomI(0, 255);
	result.b = randomI(0, 255);
    }while(determineLightness(result) < lightRange[0] || determineLightness(result) > lightRange[1]);
    
    return result;
}

float dotProduct(sf::Vector2f a, sf::Vector2f b)
{
    return a.x * b.x + a.y * b.y;
}

std::string trailingZeroes(float number, int zeroes)
{
    std::ostringstream out;
    out << std::setprecision(zeroes) << std::fixed << number;
    return out.str();
}

std::vector<sf::Color> colorGradient(sf::Color start, sf::Color end, int stepTotal)
{
    std::vector<sf::Color> result;

    result.emplace_back(start);
    
    for(int i = 1; i < stepTotal-1; ++i)
    {
	int r = start.r + ((int)std::round(i * ((float)end.r - (float)start.r) / ((float)stepTotal-1)));
	int g = start.g + ((int)std::round(i * ((float)end.g - (float)start.g) / ((float)stepTotal-1)));
	int b = start.b + ((int)std::round(i * ((float)end.b - (float)start.b) / ((float)stepTotal-1)));

	result.emplace_back(r, g, b);
    }
    
    result.emplace_back(end);

    return result;
    
}

float colorValue(float point)
{
    point = modulo(point, 1.f);
    
    if(point <= 1.f/6.f) return point*6.f;
    if(point <= 3.f/6.f) return 1.f;
    if(point <= 4.f/6.f) return (4.f/6.f - point)*6.f;
    return 0.f;
}

sf::Color colorFromRange(float point)
{
    return sf::Color(std::round(255.f * colorValue(point + 2.f/6.f)),
		     std::round(255.f * colorValue(point          )),
		     std::round(255.f * colorValue(point - 2.f/6.f)));
}

std::vector<sf::Color> generatePalette(int colorTotal)
{
    std::vector<sf::Color> result;

    for(int i = 0; i < colorTotal; ++i)
    {
	result.emplace_back(colorFromRange((float)i/(float)colorTotal));
	std::cout << i << " = ("
		  << (int)result.back().r << ", "
		  << (int)result.back().g << ", "
		  << (int)result.back().b << ")\n";
    }

    return result;
}

void printVector(sf::Vector2i a, bool enter)
{
    std::cout << "{" << a.x << ", " << a.y << "}";
    if(enter) std::cout << std::endl;
}

void printVector(sf::Vector2f a, bool enter)
{
    std::cout << "{" << a.x << ", " << a.y << "}";
    if(enter) std::cout << std::endl;
}
