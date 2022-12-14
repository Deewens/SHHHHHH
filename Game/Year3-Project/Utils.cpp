#include "Utils.h"

#include <iostream>

/// <summary>
/// Generate a random number between two values
/// </summary>
int Utils::generateRandomNumber(int from, int to)
{
    std::random_device rd;
    std::minstd_rand gen(rd());

    std::uniform_int_distribution<> rand(from, to);
    return rand(gen);
}

void Utils::playRandomSound(std::vector<sf::Sound*> &sounds)
{
    int random = generateRandomNumber(0, (int) sounds.size()-1);
    sf::Sound* sound = sounds.at(random);
    sound->play();
}

float Utils::getDistanceBetweenPoints(sf::Vector2f p, sf::Vector2f q)
{
    return sqrt(pow(q.x - p.x, 2) + pow(q.y - p.y, 2));
}

template<typename T>
sf::Vector2<T> Utils::normalize(sf::Vector2<T> v)
{
    return v/magnitude(v);
}

template<typename T>
float Utils::magnitude(sf::Vector2<T> v)
{
    return sqrt(v.x*v.x + v.y*v.y);
}

int Utils::vectorToNode(sf::Vector2f position)
{
    return (trunc(position.y / tileSize) * screen_Width / tileSize) + trunc(position.x / tileSize);
}

