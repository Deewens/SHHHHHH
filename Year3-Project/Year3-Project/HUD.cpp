#include "HUD.h"

HUD::HUD()
{
    setUpText();
    setUpHudOutline();
}

void HUD::update()
{    
}

void HUD::render(sf::RenderWindow& window)
{
    window.draw(m_hudOutline);
    window.draw(m_text);
}
void HUD::setUpText()
{
    if (!m_font.loadFromFile("ASSETS\\FONTS\\ariblk.ttf"))
    {
        std::cout << "HUD font was not loaded" << std::endl;
    }
    m_text.setFont(m_font);
    m_text.setFillColor(sf::Color::White);

}
void HUD::setUpHudOutline()
{
    //Setting up our hud properties 
    m_hudOutline.setSize(sf::Vector2f(screen_Width,40.0f));
    m_hudOutline.setFillColor(sf::Color::Red);
    m_hudOutline.setOutlineThickness(-.5f);
    m_hudOutline.setOutlineColor(sf::Color(0, 0, 0, 100));
    m_hudOutline.setPosition(0, 0);
}
