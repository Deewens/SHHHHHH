#ifndef YEAR3_PROJECT_CHARACTER_H
#define YEAR3_PROJECT_CHARACTER_H

#include <iostream>
#include <SFML/Graphics.hpp>

#include "Globals.h"

class Character : public sf::Drawable, public sf::Transformable
{
protected:
    float m_speed;
    sf::Texture m_texture;
    sf::Sprite m_sprite;
    int m_direction;

public:
    Character();

    sf::Vector2f getPosition();
    float getRadius();

    // Abstract methods
    virtual void update(sf::Time deltaTime) = 0;
    virtual void loadTexture();
private:
    void draw(sf::RenderTarget &target, sf::RenderStates states) const override;
};


#endif //YEAR3_PROJECT_CHARACTER_H
