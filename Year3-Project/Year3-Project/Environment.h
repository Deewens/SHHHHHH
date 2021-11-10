#pragma once
#include <SFML/Graphics.hpp>

class Environment
{
public:
	Environment();
	void render(sf::RenderWindow& t_window);
	sf::FloatRect getCollisionRect();

private:
	sf::RectangleShape rect;
};