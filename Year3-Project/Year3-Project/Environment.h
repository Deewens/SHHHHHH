#pragma once
#include <SFML/Graphics.hpp>

class Environment
{
public:
	Environment();
	void render(sf::RenderWindow& t_window);


private:
	sf::RectangleShape rect;
};