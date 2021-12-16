#pragma once

#include <fstream>

#include <SFML/Graphics.hpp>
#include <iostream>
#include "Globals.h"
#include "Character.h"
#include "Animation.h"
#include "json.hpp"
#include <math.h>

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

    Direction m_movementDir;
    sf::Vector2f m_velocity;

    void unitVector(sf::Vector2f& t_vector, float dt);
    void move(float dt);
    void setDirection(int t_direction);
    void boundryCheck();
    sf::Vector2f getVelocity();


    sf::RectangleShape m_powerBar;

    sf::RectangleShape m_powerthrow;

    bool m_powerBarVisible = false;


    sf::Vector2f m_powerBarSize = { 50,5 };
    sf::Vector2f m_powerSize = { 0,4 };

public:

    Player();

    void update(sf::Time deltaTime) override;

    void processEvents(sf::Event event);

    void awayFrom(sf::Vector2f t_obstacle);

    void renderPowerBar(sf::RenderWindow& t_window);

    sf::Sprite getSprite();
    
    bool m_readyToTHrow[2] = { false,false };

    float m_throwPower();

    bool m_throw[2] = { false,false };

};

