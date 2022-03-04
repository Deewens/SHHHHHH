#include "Menu.h"

void Menu::draw(sf::RenderTarget &target, sf::RenderStates states) const
{

    if (m_gameState == GameState::MENU)
    {
        target.draw(m_backgroundSprite);

        for (int i = 0; i < 4; i++)
        {
            target.draw(MainMenuButtons[i].shape);
            target.draw(MainMenuButtons[i].text);
        }        
    }
    else if (m_gameState == GameState::GAMEPLAY)
    {
        target.draw(GameBackButton.shape);
        target.draw(GameBackButton.text);
    }
    else if (m_gameState == GameState::HELP)
    {
        target.draw(GameBackButton.shape);
        target.draw(GameBackButton.text);
        target.draw(gameText);
    }    
    else if (m_gameState == GameState::PAUSE)
    {

        target.draw(m_pauseRect);
        target.draw(GameBackButton1.shape);
        target.draw(GameBackButton1.text);
        target.draw(GameBackButton2.shape);
        target.draw(GameBackButton2.text);
    }
    else if (m_gameState == GameState::LOSE)
    {
        target.draw(GameBackButton.shape);
        target.draw(GameBackButton.text);
        target.draw(gameText);
    }
    else if (m_gameState == GameState::WINLEVEL)
    {
        target.draw(GameBackButton.shape);
        target.draw(GameBackButton.text);
        target.draw(gameText);

        target.draw(GameBackButton3.shape);
        target.draw(GameBackButton3.text);
        target.draw(gameText);
    }
    else if (m_gameState == GameState::WINGAME)
    {
        target.draw(GameBackButton.shape);
        target.draw(GameBackButton.text);
        target.draw(gameText);
    }
}
