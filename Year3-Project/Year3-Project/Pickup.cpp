#include "Pickup.h"

Pickup::Pickup()
{
	rect.setSize(sf::Vector2f(50, 50));
	rect.setOrigin(sf::Vector2f(25, 25));
	rect.setFillColor(sf::Color::Cyan);
	rect.setPosition(sf::Vector2f(150, 150));
}

void Pickup::render(sf::RenderWindow& t_window)
{
	t_window.draw(rect);
}

sf::Vector2f Pickup::getPosition()
{
	return rect.getPosition();
}

float Pickup::getRadius()
{
	return rect.getOrigin().x;
}
