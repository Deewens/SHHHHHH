#pragma once
#include <SFML/Graphics.hpp>
#include "Globals.h"

class Environment
{
public:
	Environment();
	Environment(int t_tileCode, int t_rows, int t_cols, int t_tileTypeCode);
	void render(sf::RenderWindow& t_window);
	sf::FloatRect getCollisionRect();

	void impassable();

private:
	sf::RectangleShape rect;
};