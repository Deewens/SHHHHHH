#include "Menu.h"

void Menu::draw(sf::RenderTarget &target, sf::RenderStates states) const
{
    if (m_gameState == GameState::MENU)
    {
        std::cout << "test" << std::endl;
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
    else if (m_gameState == GameState::OPTIONS)
    {
        target.draw(GameBackButton.shape);
        target.draw(GameBackButton.text);
        target.draw(gameText);
    }
}
