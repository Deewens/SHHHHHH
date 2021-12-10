#include "HUD.h"

HUD::HUD()
{
    setUpText();
    setUpHudOutline();
    setUpSprite();
}

void HUD::update(sf::Vector2f t_position)
{
    m_hudOutlineRect.setPosition(t_position.x,t_position.y+screen_Height/4-10);
    m_hudOutlineCircle.setPosition(t_position.x - screen_Width/4 + m_circleRadious, t_position.y + screen_Height / 4-20);
    m_backpackSprite.setPosition(t_position.x - screen_Width / 4 + m_circleRadious, t_position.y + screen_Height / 4 - 20);

    m_hudBottleSprite[0].setPosition(m_backpackSprite.getPosition()+sf::Vector2f(-10,2));
    m_hudBottleSprite[1].setPosition(m_backpackSprite.getPosition() + sf::Vector2f(6, 2));

}

void HUD::render(sf::RenderWindow& window)
{
    window.draw(m_hudOutlineRect);
    window.draw(m_hudOutlineCircle);
    window.draw(m_backpackSprite);
    if (m_pickUpHud[0] == true)
    {
        window.draw(m_hudBottleSprite[0]);
    }
    if (m_pickUpHud[1] == true)
    {
        window.draw(m_hudBottleSprite[1]);
    }
}
void HUD::setUpText()
{
    if (!m_hudFont.loadFromFile("ASSETS\\FONTS\\ariblk.ttf"))
    {
        std::cout << "HUD font was not loaded" << std::endl;
    }
    m_hudText.setFont(m_hudFont);
}
void HUD::setUpHudOutline()
{
    m_hudOutlineRect.setSize(m_rectSize);
    m_hudOutlineRect.setOrigin(m_rectSize.x / 2 , m_rectSize.y/2);
    m_hudOutlineRect.setFillColor(sf::Color::White);

    m_hudOutlineCircle.setRadius(m_circleRadious);
    m_hudOutlineCircle.setOrigin(m_circleRadious, m_circleRadious);
    m_hudOutlineCircle.setFillColor(sf::Color::White);
}

void HUD::setUpSprite()
{
    if (!m_backpackTexture.loadFromFile("ASSETS\\IMAGES\\BackPack.png"))
    {
        // simple error message if previous call fails
        std::cout << "problem loading backpack texture" << std::endl;
    }
    m_backpackSprite.setTexture(m_backpackTexture);
    m_backpackSprite.setOrigin(m_backpackSprite.getGlobalBounds().left +500, m_backpackSprite.getGlobalBounds().top+m_backpackSprite.getGlobalBounds().height - 390);
    m_backpackSprite.setScale(0.065, 0.065);

    if (!m_hudBottleTexture.loadFromFile("ASSETS\\IMAGES\\Bottle.png"))
    {
        // simple error message if previous call fails
        std::cout << "problem loading backpack texture" << std::endl;
    }

    for (int i = 0; i < 2; i++)
    {
        m_hudBottleSprite[i].setTexture(m_hudBottleTexture);
        m_hudBottleSprite[i].setScale(0.015, 0.015);
    }    

}

