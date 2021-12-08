#include "Button.h"

Button::Button()
{
	m_button.setPosition(screen_Width, screen_Height - 100);
	m_button.setSize(sf::Vector2f(menu_Width, 100));
	m_button.setFillColor(sf::Color::Green);

	if (!m_font.loadFromFile("ASSETS/FONTS/ariblk.ttf"))
	{
		std::cout << "font not loading";
	}

	m_buttonText.setString("oof");
	m_buttonText.setFont(m_font);
	m_buttonText.setPosition(screen_Width, screen_Height - 100);
	m_buttonText.setCharacterSize(30);
	m_buttonText.setFillColor(sf::Color::Red);
	m_buttonText.setOrigin(m_buttonText.getGlobalBounds().width / 2, m_buttonText.getGlobalBounds().height);
	m_buttonText.setPosition(screen_Width + menu_Width / 2, screen_Height - 50);
	isSprite = false;
}

Button::Button(sf::Vector2f t_location, sf::Vector2f t_size, sf::Color t_color, std::string t_text, int t_characterSize, sf::Color t_textColor)
{
	if (!m_font.loadFromFile("ASSETS/FONTS/ariblk.ttf"))
	{
		std::cout << "font not loading";
	}

	m_button.setPosition(t_location);
	m_button.setSize(t_size);
	m_button.setFillColor(t_color);

	m_buttonText.setString(t_text);
	m_buttonText.setFont(m_font);
	m_buttonText.setCharacterSize(t_characterSize);
	m_buttonText.setFillColor(t_textColor);
	m_buttonText.setOrigin(m_buttonText.getGlobalBounds().width / 2, m_buttonText.getGlobalBounds().height);
	m_buttonText.setPosition(t_location + (t_size / 2.0f));
	
	isSprite = false;
}

Button::Button(sf::Vector2f t_location, sf::Vector2f t_size, sf::Texture t_texture) : m_texture(t_texture)
{
	m_sprite.setTexture(m_texture);
	m_sprite.setPosition(t_location);
	m_sprite.setScale(t_size.x / m_texture.getSize().x, t_size.y / m_texture.getSize().y);
	isSprite = true;
}

void Button::render(sf::RenderWindow& t_window)
{
	if (isSprite)
	{
		t_window.draw(m_sprite);
	}
	else
	{
		t_window.draw(m_button);
		t_window.draw(m_buttonText);
	}
}
