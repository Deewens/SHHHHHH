#pragma once
#include <SFML/Graphics.hpp>
#include "Globals.h"

class Goal
{
public:
	Goal(int t_tileCode);
	void render(sf::RenderWindow& t_window);
	sf::FloatRect getCollisionRect();
private:
	sf::RectangleShape goalZone;
};