#include "Environment.h"

Environment::Environment()
{
	rect.setSize(sf::Vector2f(50, 50));
	rect.setOrigin(sf::Vector2f(25, 25));
	rect.setFillColor(sf::Color::Red);
	rect.setPosition(sf::Vector2f(350, 150));
}

Environment::Environment(int t_tileCode, int t_rows, int t_cols, int t_tileTypeCode)
{
	rect.setSize(sf::Vector2f(tileSize, tileSize));
	rect.setOrigin(sf::Vector2f(tileSize / 2, tileSize / 2));
	rect.setFillColor(sf::Color::Red);
	float col = t_tileCode % t_cols;
	float row = (t_tileCode - col) / t_cols;
	col = (col * tileSize) + (tileSize / 2);
	row = (row * tileSize) + (tileSize / 2);
	rect.setPosition(sf::Vector2f(col, row));
}

void Environment::render(sf::RenderWindow& t_window)
{
	t_window.draw(rect);
}

sf::FloatRect Environment::getCollisionRect()
{
	return rect.getGlobalBounds();
}

void Environment::impassable()
{
	
}
