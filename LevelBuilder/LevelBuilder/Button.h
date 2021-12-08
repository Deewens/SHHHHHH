#pragma once
#include <SFML/Graphics.hpp>
#include "Globals.h"
#include <iostream>

class Button
{
public:
	Button();
	Button(sf::Vector2f t_location, sf::Vector2f t_size, sf::Color t_color, std::string t_text, int t_characterSize, sf::Color t_textColor);
	Button(sf::Vector2f t_location, sf::Vector2f t_size, sf::Texture t_texture);
	void render(sf::RenderWindow& t_window);
private:
	sf::Sprite m_sprite;
	sf::Texture m_texture;
	sf::RectangleShape m_button;
	sf::Text m_buttonText;
	sf::Font m_font;

	bool isSprite;
};

