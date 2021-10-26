#pragma once
#include<SFML/Graphics.hpp>
#include<iostream>
#include"Globals.h"
class HUD
{
public:
    
    HUD();

   void update();

    /// <summary>
    /// @brief Draws the HUD outline and text.
    ///
    /// </summary>
    /// <param name="window">The SFML Render window</param>
    void render(sf::RenderWindow& window);

    void setUpText();

    void setUpHudOutline();

private:

    sf::Font m_font;

    sf::Text m_text;

    // A simple background shape for the HUD.
    sf::RectangleShape m_hudOutline;

};

