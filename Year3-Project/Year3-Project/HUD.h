#pragma once

#include<SFML/Graphics.hpp>
#include<iostream>
#include"Globals.h"

class HUD
{
public:
    
    HUD();

    
    void update(sf::Vector2f t_position);
    
    void render(sf::RenderWindow& window);

    void setUpText();

    void setUpHudOutline();

    void setUpSprite();

    bool m_pickUpHud[2] = { false,false };

private:

    sf::Font m_hudFont;

    sf::Text m_hudText;

    sf::RectangleShape m_hudOutlineRect;

    sf::CircleShape m_hudOutlineCircle;

    sf::Vector2f m_rectSize = { screen_Width / 1.5,50.0f };

    float m_circleRadious = 70.0f;

    sf::Texture m_backpackTexture;
    sf::Sprite m_backpackSprite;

    sf::Texture m_hudBottleTexture;
    sf::Sprite m_hudBottleSprite[2];

};

