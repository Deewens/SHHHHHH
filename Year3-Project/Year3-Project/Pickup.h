#pragma once
#include <SFML/Graphics.hpp>
#include <iostream>

class Pickup
{
public:
	Pickup();
	void render(sf::RenderWindow& t_window);

	sf::Vector2f getPosition();
	float getRadius();
private:
	sf::RectangleShape rect;
};

