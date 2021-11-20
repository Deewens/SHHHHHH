#pragma once
#include <SFML/Graphics.hpp>
#include "Globals.h"
#include <fstream>
#include "json.hpp"
#include "GroundTypes.h"

class Environment
{
public:
	Environment(sf::RectangleShape& t_rect, int t_tileCode, int t_rows, int t_cols, float t_rotation = 0, bool t_impassable = true);
    Environment(sf::Texture& t_texture, const std::string& t_sprite, int t_tileCode, int t_rows, int t_cols, float t_rotation = 0, bool t_impassable = false);
	void render(sf::RenderWindow& t_window);
	sf::FloatRect getCollisionRect();

	bool isImpassable() const;

private:
	sf::RectangleShape m_rect;
    sf::Sprite m_sprite;

    float m_rotation;
    bool m_impassable; // If characters can pass through the element or not
};