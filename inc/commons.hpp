#pragma once

#include <iostream>
#include <vector>
#include <memory>
#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/Audio.hpp>


template <typename T>
T& atCoords(std::vector< std::vector<T> >& data, sf::Vector2i coords)
{
    return data[coords.x][coords.y];
}

int modulo(int a, int b);

float modulo(float a, float b);

int randomI(int min, int max);

// random integer with target - n numbers generated within range and then one closest to target is chosen
int randomIwT(int min, int max, int target, int attempts);

float randomF(float min, float max, float step = 0.01);

float randomFwT(float min, float max, float target, int attempts, float step = 0.01);

float radToDeg(float radians);

float normalizeAngle(float angle);

float length(sf::Vector2f input);

float distance(sf::Vector2f a, sf::Vector2f b);

float determineLightness(sf::Color input);
    
sf::Color randomColor(std::vector<float> lightRange);
    
float dotProduct(sf::Vector2f a, sf::Vector2f b);

std::string trailingZeroes(float number, int zeroes = 2);

std::vector<sf::Color> colorGradient(sf::Color start, sf::Color end, int stepTotal);

std::vector<sf::Color> generatePalette(int colorTotal);

void printVector(sf::Vector2i a, bool enter);

void printVector(sf::Vector2f a, bool enter);
