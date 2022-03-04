#include "Goal.h"

Goal::Goal(int t_tileCode)
{
	float col = t_tileCode % (screen_Width / tileSize);
	float row = (t_tileCode - col) / (screen_Width / tileSize);
	goalZone.setPosition(col * tileSize, row * tileSize);
	goalZone.setSize(sf::Vector2f(tileSize, tileSize));
	goalZone.setFillColor(sf::Color(0, 255, 0, 128));
}

void Goal::render(sf::RenderWindow& t_window)
{
	t_window.draw(goalZone);
}

sf::FloatRect Goal::getCollisionRect()
{
	return goalZone.getGlobalBounds();
}
