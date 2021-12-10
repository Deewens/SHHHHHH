#pragma once
#include <SFML/Graphics.hpp>
#include "Globals.h"
#include <iostream>

class Button
{
public:
	Button();
	Button(sf::Vector2f t_location, sf::Vector2f t_size, sf::Color t_color, std::string t_text, int t_characterSize, sf::Color t_textColor);
	Button(sf::Vector2f t_location, sf::Vector2f t_size, sf::Texture t_texture, std::string t_spriteName);
	void render(sf::RenderWindow& t_window);

	bool isInside(sf::Vector2i t_click);

	void moveUp(float t_speed);
	void moveDown(float t_speed);
	void setSelected(bool t_selected);

	sf::Texture& getTexture();
	std::string getSpriteName();
private:
	sf::Sprite m_sprite;
	sf::Texture m_texture;
	sf::RectangleShape m_button;
	sf::Text m_buttonText;
	sf::Font m_font;

	bool isSprite;
	bool isSelected = false;

	sf::FloatRect buttonStartSize;
	sf::FloatRect buttonSize;

	float scrollHeight;

	void checkSpriteVisible();

	std::string m_spriteName = "";
	sf::Color m_buttonColor = sf::Color::Transparent;
};

