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
    MenuButton GameBackButton1;
    MenuButton GameBackButton2;
    MenuButton GameBackButton3;



    sf::Text gameText;

    sf::Texture m_backgroundTexture;
    sf::Sprite m_backgroundSprite;

    sf::RectangleShape m_pauseRect;


    void Init()
    {       
        font.loadFromFile("ASSETS\\FONTS\\ariblk.ttf");  

        if (!m_backgroundTexture.loadFromFile("ASSETS\\IMAGES\\MenuBackground.png"))
        {
            std::cout << "problem loading Menu Background" << std::endl;
        }
        m_backgroundSprite.setTexture(m_backgroundTexture);
        m_backgroundSprite.setScale(1.1, 1.1);
        m_backgroundSprite.setPosition(300, 10);

        GameBackButton.Init(sf::Vector2f(0, 0), font,sf::Vector2f(250, 40), sf::Vector2f(0, 0));
        GameBackButton3.Init(sf::Vector2f(0, 60), font, sf::Vector2f(250, 40), sf::Vector2f(30, 0));

        GameBackButton.text.setString("Back To Menu");

        GameBackButton1.text.setString("Press P to Resume");

        GameBackButton2.text.setString("Press E to MainMenu");

        GameBackButton3.text.setString("Next Level");


        gameText.setFont(font);
        //gameText.setString("Coming Soon.....");
        gameText.setPosition(200, 300);
        for (int i = 0; i < 3; i++)
        {
            MainMenuButtons[i].Init(sf::Vector2f(300, 80 + (i * 110)), font,sf::Vector2f(250, 100), sf::Vector2f(10, 30));
        }
        MainMenuButtons[0].text.setString("Play");
        MainMenuButtons[1].text.setString("Help");
        MainMenuButtons[2].text.setString("Exit"); 

        m_pauseRect.setSize(sf::Vector2f(screen_Width / 2.5, screen_Height / 2.5));
        m_pauseRect.setFillColor(sf::Color(225, 0, 0, 100));
        m_pauseRect.setOrigin(screen_Width / 5, screen_Height / 5);
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
                m_gameState = GameState::HELP;
            }            
            if (MainMenuButtons[2].pressed == true)
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
        else if (m_gameState == GameState::HELP)
        {
            GameBackButton.Update(mousePos);
            if (GameBackButton.pressed == true)
            {
                m_gameState = GameState::MENU;
            }           
        }
        else if (m_gameState == GameState::PAUSE)
        {
            GameBackButton1.Update(mousePos);
            if (GameBackButton1.pressed == true)
            {
                m_gameState = GameState::MENU;
            }
        }        
        else if (m_gameState == GameState::LOSE)
        {
            GameBackButton.Update(mousePos);
            if(GameBackButton.pressed == true)
            {
                m_gameState = GameState::MENU;
            }
        }
        else if (m_gameState == GameState::WINLEVEL)
        {
            GameBackButton.Update(mousePos);
            GameBackButton3.Update(mousePos);

            if (GameBackButton.pressed == true)
            {
                m_gameState = GameState::MENU;
            }
            if (GameBackButton3.pressed == true)
            {
                std::cout << "next level" << std::endl;
            }
        }
        else if (m_gameState == GameState::WINGAME)
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

