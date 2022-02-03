#pragma once
#include <SFML/Graphics.hpp>
#include "Button.h"
class Tile
{
public:
	Tile(Button* t_tile, int t_tileNum, sf::Texture& t_texture);
	void render(sf::RenderWindow& t_window);
	void rotate();
private:
	sf::Sprite m_sprite;
	std::string m_spriteName;
	int m_tileNum;
	bool m_passable;
};
