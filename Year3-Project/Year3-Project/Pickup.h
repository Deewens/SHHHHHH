#pragma once
#include <SFML/Graphics.hpp>
#include <iostream>

class Pickup
{
public:
	Pickup();
	void render(sf::RenderWindow& t_window);
private:
	sf::RectangleShape rect;
};

