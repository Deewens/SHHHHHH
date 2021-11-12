#pragma once

#include <fstream>

#include <SFML/Graphics.hpp>
#include <iostream>
#include "Globals.h"
#include "Character.h"
#include "Animation.h"
#include "json.hpp"

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

    bool m_pausePressed = false;

    Animation m_crouchingAnim;
    Animation m_walkingAnim;
    Animation m_runningAnim;

    Animation m_idlingAnim;
    Animation m_throwingAnim;

    Direction m_isMoving;
    sf::Vector2f m_velocity;

public:

    Player();

    void setDirection(int t_direction);

    void update(sf::Time deltaTime) override;

    void move(float dt);

    void processEvents(sf::Event event);

    void awayFrom(sf::Vector2f t_obstacle);

    sf::Vector2f getVelocity();

    void boundryCheck();
};

