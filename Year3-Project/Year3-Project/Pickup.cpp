#include "Pickup.h"

Pickup::Pickup()
{
	rect.setSize(sf::Vector2f(20, 20));
	rect.setOrigin(sf::Vector2f(10, 10));
	rect.setFillColor(sf::Color::Cyan);
	rect.setPosition(sf::Vector2f(150, 150));	
}

sf::Vector2f Pickup::getPosition()
{
	return rect.getPosition();
}

float Pickup::getRadius()
{
	return rect.getOrigin().x;
}

void Pickup::setUpSprite()
{
	if (!m_bottleTexture.loadFromFile("ASSETS\\IMAGES\\Bottle.png"))
	{
		// simple error message if previous call fails
		std::cout << "problem loading backpack texture" << std::endl;
	}
	if (!m_canTexture.loadFromFile("ASSETS\\IMAGES\\Can.png"))
	{
		// simple error message if previous call fails
		std::cout << "problem loading backpack texture" << std::endl;
	}

	m_bottleSprite.setTexture(m_bottleTexture);
	m_canSprite.setTexture(m_canTexture);

}

void Pickup::draw(sf::RenderTarget &target, sf::RenderStates states) const
{
    target.draw(rect);
}
