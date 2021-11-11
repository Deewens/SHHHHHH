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

    bool m_pausePressed = false;

    Direction m_isMoving;
    sf::Vector2f m_velocity;

    sf::Texture m_idlingTexture;
    sf::Texture m_runningTexture;

    AnimatedSprite m_animatedSprite;
    Animation* m_currentAnimation;
    Animation m_idlingAnimation;

public:

    Player();

    void setDirection(int t_direction);

    void update(sf::Time deltaTime) override;

    void move(float dt);

    void processEvents(sf::Event event);

    void awayFrom(sf::Vector2f t_obstacle);

    sf::Vector2f getVelocity();

    void boundryCheck();

    void loadTextures();

    void draw(sf::RenderTarget &target, sf::RenderStates states) const override;
};

