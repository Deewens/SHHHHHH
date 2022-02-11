#pragma once

#include <SFML/Graphics.hpp>

struct NodeData
{
    int id = 0;
    sf::Vector2f position;
    bool isPassable = true;
    float cost = 0.0f;
};


