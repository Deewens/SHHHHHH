#include "Character.h"


Character::Character()
{
    m_speed = 2;
    m_direction = SOUTHEAST;
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

void Character::draw(sf::RenderTarget &target, sf::RenderStates states) const
{
    //target.draw(m_sprite);
    //target.draw(m_animatedSprite);
}

