#include "Enemy.h"

Enemy::Enemy()
{
    m_speed = 2;
    Enemy::loadImage();
    Enemy::setDirection(SOUTH);

    m_sprite.setOrigin(17.5, 21.5);
    m_sprite.setPosition(300, 300);
    m_sprite.setTextureRect(sf::IntRect(424, 0, 35, 43));

    visionConeInit(VISION_CONE_ANGLE);
}

void Enemy::setDirection(int t_direction)
{   
    switch (t_direction)
    {
    case NORTH:
        m_sprite.setRotation(270);
        break;
    case SOUTH:
        m_sprite.setRotation(90);
        break;
    case EAST:
        m_sprite.setRotation(0);
        break;
    case WEST:
        m_sprite.setRotation(180);
        break;
    case NORTHWEST:
        m_sprite.setRotation(225);
        break;
    case NORTHEAST:
        m_sprite.setRotation(315);
        break;
    case SOUTHWEST:
        m_sprite.setRotation(135);
        break;
    case SOUTHEAST:
        m_sprite.setRotation(45);
        break;
    }
}

void Enemy::update(float dt)
{
}

void Enemy::visionConeInit(float t_angle)
{
    coneVision[0].position = sf::Vector2f(m_sprite.getPosition());
    coneVision[0].color = sf::Color::Green;

    coneVision[1].position = sf::Vector2f(VISION_CONE_LENGTH * std::sin(m_sprite.getRotation()-VISION_CONE_ANGLE+45) + m_sprite.getPosition().x,
                                          VISION_CONE_LENGTH * std::cos(m_sprite.getRotation()-VISION_CONE_ANGLE+45) + m_sprite.getPosition().y);
    coneVision[1].color = sf::Color::Transparent;


    coneVision[2].position = sf::Vector2f(VISION_CONE_LENGTH * std::sin(m_sprite.getRotation()+VISION_CONE_ANGLE+45) + m_sprite.getPosition().x,
                                          VISION_CONE_LENGTH * std::cos(m_sprite.getRotation()+VISION_CONE_ANGLE+45) + m_sprite.getPosition().y);
    coneVision[2].color = sf::Color::Transparent;

  }

void Enemy::renderVisionCone(sf::RenderWindow& t_window)
{
     t_window.draw(coneVision,3,sf::Triangles);
}
