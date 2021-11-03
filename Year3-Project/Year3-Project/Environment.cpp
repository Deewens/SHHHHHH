#include "Environment.h"

Environment::Environment()
{
	rect.setSize(sf::Vector2f(50, 50));
	rect.setOrigin(sf::Vector2f(25, 25));
	rect.setFillColor(sf::Color::Red);
	rect.setPosition(sf::Vector2f(350, 150));
}

void Environment::render(sf::RenderWindow& t_window)
{
	t_window.draw(rect);
}
