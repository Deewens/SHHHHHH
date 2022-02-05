#ifndef YEAR3_PROJECT_CHARACTER_H
#define YEAR3_PROJECT_CHARACTER_H

#include <iostream>
#include <SFML/Graphics.hpp>

#include "Globals.h"
#include "Utils.h"

class Character : public sf::Drawable
{
protected:
    float m_speed;
    sf::Texture m_texture;
    sf::Sprite m_sprite;
    int m_direction;

public:
    Character();

    float getRadius();
    float getDistance(Character& t_character);

    sf::Vector2f getPosition();
    void setPosition(float x, float y);
    void setPosition(sf::Vector2f position);

    float getRotation();

    // Abstract methods
    virtual void update(sf::Time deltaTime);
    virtual void loadTexture();
private:
    void draw(sf::RenderTarget &target, sf::RenderStates states) const override;
};


#endif //YEAR3_PROJECT_CHARACTER_H
