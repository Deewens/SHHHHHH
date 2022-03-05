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

    sf::Texture m_loseTexture;
    sf::Sprite m_loseSprite;

    sf::Texture m_lvlPassTexture;
    sf::Sprite m_lvlPassSprite;

    sf::Texture m_gameFinishTexture;
    sf::Sprite m_gameFinishSprite;

    sf::RectangleShape m_pauseRect;


    void Init()
    {       
        font.loadFromFile("ASSETS\\FONTS\\ariblk.ttf");  

        if (!m_backgroundTexture.loadFromFile("ASSETS\\IMAGES\\MenuBackground.png"))
        {
            std::cout << "problem loading Menu Background" << std::endl;
        }
        if (!m_loseTexture.loadFromFile("ASSETS\\IMAGES\\You-Died.png"))
        {
            std::cout << "problem loading lose Background" << std::endl;
        }
        if (!m_lvlPassTexture.loadFromFile("ASSETS\\IMAGES\\Passed.png"))
        {
            std::cout << "problem loading win Background" << std::endl;
        }
        if (!m_gameFinishTexture.loadFromFile("ASSETS\\IMAGES\\Survived.png" , sf::IntRect(90, 0, 650, 200)))
        {
            std::cout << "problem loading win Background" << std::endl;
        }
        m_backgroundTexture.setSmooth(true);
        m_backgroundSprite.setTexture(m_backgroundTexture);
        m_backgroundSprite.setScale(1.1, 1.1);
        m_backgroundSprite.setPosition(300, 10);

        m_loseTexture.setSmooth(true);
        m_loseSprite.setTexture(m_loseTexture);
        m_loseSprite.setScale(0.65, 0.65);

        m_lvlPassTexture.setSmooth(true);
        m_lvlPassSprite.setTexture(m_lvlPassTexture);
        m_lvlPassSprite.setScale(2, 2);
        m_lvlPassSprite.setPosition(100, 100);

        m_gameFinishTexture.setSmooth(true);
        m_gameFinishSprite.setTexture(m_gameFinishTexture);
        m_gameFinishSprite.setScale(1.7, 1.7);
        m_gameFinishSprite.setPosition(600, 350);
        m_gameFinishSprite.setRotation(-25);
        m_gameFinishSprite.setOrigin(350, 100);

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
            MainMenuButtons[i].Init(sf::Vector2f(0, 0 + (i * 110)), font,sf::Vector2f(250, 100), sf::Vector2f(10, 30));
        }
        MainMenuButtons[0].text.setString("Play");
        MainMenuButtons[1].text.setString("Help");
        MainMenuButtons[2].text.setString("Exit"); 

        m_pauseRect.setSize(sf::Vector2f(screen_Width / 2.5, screen_Height / 2.5));
        m_pauseRect.setFillColor(sf::Color(225, 0, 0, 100));
        m_pauseRect.setOrigin(screen_Width / 5, screen_Height / 5);
    }

    bool update(sf::Vector2f mousePos, sf::RenderWindow& t_window)
    {
        bool gameStarted = false;
        if (m_gameState == GameState::MENU)
        {
            for (int i = 0; i < 4; i++)
            {
                MainMenuButtons[i].Update(mousePos, t_window);
            }
            if (MainMenuButtons[0].pressed == true)
            {
                m_gameState = GameState::GAMEPLAY;
                gameStarted = true;
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
            GameBackButton.Update(mousePos, t_window);
            if (GameBackButton.pressed == true)
            {
                m_gameState = GameState::MENU;
            }
        }
        else if (m_gameState == GameState::HELP)
        {
            GameBackButton.Update(mousePos, t_window);
            if (GameBackButton.pressed == true)
            {
                m_gameState = GameState::MENU;
            }           
        }
        else if (m_gameState == GameState::PAUSE)
        {
            GameBackButton1.Update(mousePos, t_window);
            if (GameBackButton1.pressed == true)
            {
                m_gameState = GameState::MENU;
            }
        }        
        else if (m_gameState == GameState::LOSE)
        {
            GameBackButton.Update(mousePos, t_window);
            if(GameBackButton.pressed == true)
            {
                m_gameState = GameState::MENU;
            }
        }
        else if (m_gameState == GameState::WINLEVEL)
        {
            GameBackButton.Update(mousePos, t_window);
            GameBackButton3.Update(mousePos, t_window);

            if (GameBackButton.pressed == true)
            {
                m_gameState = GameState::MENU;
            }
            if (GameBackButton3.pressed == true)
            {
                m_gameState = GameState::GAMEPLAY;
            }
        }
        else if (m_gameState == GameState::WINGAME)
        {
            GameBackButton.Update(mousePos, t_window);
            if (GameBackButton.pressed == true)
            {
                m_gameState = GameState::MENU;
            }
        }
        return gameStarted;
    }   

private:
    void draw(sf::RenderTarget &target, sf::RenderStates states) const final;
};

