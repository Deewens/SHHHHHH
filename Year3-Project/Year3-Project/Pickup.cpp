#include "Pickup.h"

Pickup::Pickup()
{
	rect.setSize(sf::Vector2f(20, 20));
	rect.setOrigin(sf::Vector2f(10, 10));
	rect.setFillColor(sf::Color::Cyan);
	rect.setPosition(sf::Vector2f(150, 150));	
}

Pickup::Pickup(int t_gridNum, const PickUpType& m_type)
{
	if (m_type == PickUpType::BOTTLE)
	{
		setUpSpriteBottle();
	}
	if (m_type == PickUpType::CAN)
	{
		setUpSpriteCan();
	}
}

sf::Vector2f Pickup::getPosition()
{
	return rect.getPosition();
}

float Pickup::getRadius()
{
	return rect.getOrigin().x;
}

void Pickup::setUpSpriteBottle()
{
	if (!m_bottleTexture.loadFromFile("ASSETS\\IMAGES\\Bottle.png"))
	{
		// simple error message if previous call fails
		std::cout << "problem loading backpack texture" << std::endl;
	}
	m_bottleSprite.setTexture(m_bottleTexture);
	m_bottleSprite.setOrigin(m_bottleSprite.getGlobalBounds().width / 2, m_bottleSprite.getGlobalBounds().height / 2);
}

void Pickup::setUpSpriteCan()
{
	if (!m_canTexture.loadFromFile("ASSETS\\IMAGES\\Can.png"))
	{
		// simple error message if previous call fails
		std::cout << "problem loading backpack texture" << std::endl;
	}
	m_canSprite.setTexture(m_canTexture);
	m_canSprite.setOrigin(m_canSprite.getGlobalBounds().width / 2, m_canSprite.getGlobalBounds().height / 2);


}

void Pickup::draw(sf::RenderTarget &target, sf::RenderStates states) const
{
    target.draw(rect);
}
