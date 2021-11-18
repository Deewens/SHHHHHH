#pragma once
#include <SFML/Graphics.hpp>
#include "Globals.h"
#include <fstream>
#include "json.hpp"
#include "GroundTypes.h"

class Environment
{
public:
	Environment();
	Environment(int t_tileCode, int t_rows, int t_cols, int t_tileTypeCode);
    Environment(sf::Texture& t_texture, std::string t_sprite, int t_tileCode, int t_rows, int t_cols, int t_tileTypeCode, float rotation = 0);
	void render(sf::RenderWindow& t_window);
	sf::FloatRect getCollisionRect();

	void impassable();

private:
	sf::RectangleShape rect;

    sf::Sprite m_sprite;
};