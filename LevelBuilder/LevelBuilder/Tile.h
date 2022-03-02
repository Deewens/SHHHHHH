#pragma once
#include <SFML/Graphics.hpp>
#include "Button.h"
class Tile
{
public:
	Tile(Button* t_tile, int t_tileNum, sf::Texture& t_texture);
	void changeLayers(Button* t_tile);
	void render(sf::RenderWindow& t_window);
	void rotate();
	std::string getJsonInfo(int t_gridIndex);
	std::string getSpecialJson(int t_gridIndex);

	bool isPassable();
private:
	bool m_objectActive = false;
	sf::Sprite m_objectSprite;
	std::string m_objectSpriteName;
	bool m_groundActive = false;
	sf::Sprite m_groundSprite;
	std::string m_groundSpriteName;
	int m_tileNum;
	bool specialHere = false;
	bool m_isPassable;
};
