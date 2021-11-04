#include "Character.h"


Character::Character()
{
    m_speed = 2;
    m_direction = SOUTHEAST;

    Character::loadImage();
}

sf::Vector2f Character::getPosition()
{
    return m_sprite.getPosition();
}

float Character::getRadius()
{
    sf::Vector2f origin = m_sprite.getOrigin();
    float value = origin.x;
    if (origin.y > value)
    {
        value = origin.y;
    }
    return value;
}

void Character::loadImage()
{
    // Load a default sprite if loadImage is not overrided
    if (!m_texture.loadFromFile("ASSETS\\IMAGES\\spritesheet_characters.png"))
        std::cout << "problem loading character texture" << std::endl;

    m_sprite.setTexture(m_texture);
}

void Character::draw(sf::RenderTarget &target, sf::RenderStates states) const
{
    target.draw(m_sprite);
}

