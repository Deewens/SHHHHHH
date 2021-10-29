#pragma once

#include <SFML/Graphics.hpp>
#include <iostream>
#include "Globals.h"
#include "Character.h"

struct Direction {
    bool up = false;
    bool down = false;
    bool right = false;
    bool left = false;
};

class Player : public Character
{

private:
    bool m_isAlive = true;
    Direction m_isMoving;

public:

    Player();

    void setDirection(int t_direction);

    void update(float dt) override;

    void move(float dt);

    void processEvents(sf::Event event);

};

