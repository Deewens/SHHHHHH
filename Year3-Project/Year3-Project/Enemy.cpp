#include "Enemy.h"

Enemy::Enemy()
{
    m_speed = 2;
    Enemy::loadImage();
    Enemy::setDirection(SOUTHEAST);

    m_sprite.setOrigin(19, 21.5);
    m_sprite.setPosition(100, 100);
    m_sprite.setTextureRect(sf::IntRect(424, 0, 35, 43));
}

void Enemy::loadImage()
{
    if (!m_texture.loadFromFile("ASSETS\\IMAGES\\spritesheet_characters.png"))
        std::cout << "problem loading enemy texture" << std::endl;

    m_sprite.setTexture(m_texture);
}

void Enemy::setDirection(int t_direction)
{
    m_direction = t_direction;
    if (t_direction == NORTH)
    {
        m_sprite.setRotation(270);
        m_sprite.move(0, -m_speed);
    }
    if (t_direction == SOUTH)
    {
        m_sprite.setRotation(90);
        m_sprite.move(0, m_speed);
    }
    if (t_direction == EAST)
    {
        m_sprite.setRotation(0);
        m_sprite.move(m_speed, 0);

    }
    if (t_direction == WEST)
    {
        m_sprite.setRotation(180);
        m_sprite.move(-m_speed, 0);

    }
    if (t_direction == NORTHWEST)
    {
        m_sprite.setRotation(225);
    }
    if (t_direction == NORTHEAST)
    {
        m_sprite.setRotation(315);
    }
    if (t_direction == SOUTHWEST)
    {
        m_sprite.setRotation(135);
    }
    if (t_direction == SOUTHEAST)
    {
        m_sprite.setRotation(45);
    }
}

void Enemy::update()
{

}

void Enemy::render(sf::RenderWindow &t_window)
{
    t_window.draw(m_sprite);
}
