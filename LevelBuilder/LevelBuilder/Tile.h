#pragma once
#include <SFML/Graphics.hpp>
#include "Button.h"
class Tile
{
public:
	Tile(Button* t_tile, int t_tileNum);
	void render(sf::RenderWindow& t_window);
private:
	sf::Sprite m_sprite;
	std::string m_spriteName;
	int m_tileNum;
	bool m_passable;
};