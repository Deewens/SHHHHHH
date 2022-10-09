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
    target.draw(m_sprite);
}

void Character::loadTexture()
{
    // Load a default sprite if loadImage is not overrided
    if (!m_texture.loadFromFile("ASSETS/IMAGES/sprite_sheets/characters_sprite_sheet.png"))
        std::cout << "problem loading character texture" << std::endl;

    m_texture.setSmooth(true);
    m_sprite.setTexture(m_texture);
}

float Character::getDistance(Character& t_character)
{
    return Utils::getDistanceBetweenPoints(this->getPosition(), t_character.getPosition());
}

void Character::update(sf::Time deltaTime) {}

void Character::setPosition(float x, float y)
{
    m_sprite.setPosition(x, y);
}

void Character::setPosition(sf::Vector2f position)
{
    m_sprite.setPosition(position);
}

float Character::getRotation()
{
    return m_sprite.getRotation();
}

