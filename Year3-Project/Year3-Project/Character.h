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

    sf::Vector2f getPosition();
    float getRadius();

    void loadImage();
    void render(sf::RenderWindow& t_window);

    // Abstract methods
    virtual void update(float dt) = 0;
};


#endif //YEAR3_PROJECT_CHARACTER_H
