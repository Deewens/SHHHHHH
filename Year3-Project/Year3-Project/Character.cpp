#include "Character.h"

Character::Character()
{
    m_speed = 2;
    m_direction = SOUTHEAST;

    Character::loadImage();
}

void Character::loadImage()
{
    // Load a default sprite if loadImage is not overrided
    if (!m_texture.loadFromFile("ASSETS\\IMAGES\\spritesheet_characters.png"))
        std::cout << "problem loading character texture" << std::endl;

    m_sprite.setTexture(m_texture);
}

