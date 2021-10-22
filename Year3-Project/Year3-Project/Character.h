#ifndef YEAR3_PROJECT_CHARACTER_H
#define YEAR3_PROJECT_CHARACTER_H

#include <iostream>
#include <SFML/Graphics.hpp>

#include "Globals.h"

class Character
{
protected:
    float m_speed;
    sf::Texture m_texture;
    sf::Sprite m_sprite;
    bool m_isMoving = false;
    int m_direction;

public:
    Character();

    // Abstract methods
    void loadImage();
    virtual void update() = 0;
    virtual void render(sf::RenderWindow& t_window) = 0;
};


#endif //YEAR3_PROJECT_CHARACTER_H
