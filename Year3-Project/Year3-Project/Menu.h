#pragma once
#include<SFML/Graphics.hpp>
#include"MenuButton.h"
#include<iostream>

class Menu : public sf::Drawable
{
public:

    sf::Font font;
    MenuButton MainMenuButtons[4];
    MenuButton GameBackButton;
    sf::Text gameText;


    void Init()
    {       
        font.loadFromFile("ASSETS\\FONTS\\ariblk.ttf");        

        GameBackButton.Init(sf::Vector2f(0, 0), font,sf::Vector2f(250, 40), sf::Vector2f(0, 0));
        GameBackButton.text.setString("Back To Menu");


        gameText.setFont(font);
        //gameText.setString("Coming Soon.....");
        gameText.setPosition(200, 300);
        for (int i = 0; i < 4; i++)
        {
            MainMenuButtons[i].Init(sf::Vector2f(300, 80 + (i * 110)), font,sf::Vector2f(250, 100), sf::Vector2f(10, 30));
        }
        MainMenuButtons[0].text.setString("Play");
        MainMenuButtons[1].text.setString("Options");
        MainMenuButtons[2].text.setString("Help");
        MainMenuButtons[3].text.setString("Exit");        
    }

    void update(sf::Vector2f mousePos)
    {

        if (m_gameState == GameState::MENU)
        {
            for (int i = 0; i < 4; i++)
            {
                MainMenuButtons[i].Update(mousePos);
            }
            if (MainMenuButtons[0].pressed == true)
            {
                m_gameState = GameState::GAMEPLAY;
            }
            if (MainMenuButtons[1].pressed == true)
            {
                m_gameState = GameState::OPTIONS;
            }
            if (MainMenuButtons[2].pressed == true)
            {
                std::cout << " 3 pressed" << std::endl;
            }
            if (MainMenuButtons[3].pressed == true)
            {
                m_gameState = GameState::EXIT;

            }
        }
        else if (m_gameState == GameState::GAMEPLAY)
        {
            GameBackButton.Update(mousePos);
            if (GameBackButton.pressed == true)
            {
                m_gameState = GameState::MENU;
            }
        }
        else if (m_gameState == GameState::OPTIONS)
        {
            GameBackButton.Update(mousePos);
            if (GameBackButton.pressed == true)
            {
                m_gameState = GameState::MENU;
            }
           
        }        
    }

private:
    void draw(sf::RenderTarget &target, sf::RenderStates states) const final;
};

