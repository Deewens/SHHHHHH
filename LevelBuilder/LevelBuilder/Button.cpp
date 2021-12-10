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

	scrollHeight = 0;
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

	buttonStartSize.left = t_location.x;
	buttonStartSize.top = t_location.y;
	buttonStartSize.width = t_size.x;
	buttonStartSize.height = t_size.y;

	buttonSize = buttonStartSize;
	scrollHeight = t_location.y;
}

Button::Button(sf::Vector2f t_location, sf::Vector2f t_size, sf::Texture t_texture) : m_texture(t_texture)
{
	m_sprite.setTexture(m_texture);
	m_sprite.setPosition(t_location);
	m_sprite.setScale(t_size.x / m_texture.getSize().x, t_size.y / m_texture.getSize().y);
	isSprite = true;

	buttonStartSize.left = 0;
	buttonStartSize.top = 0;
	buttonStartSize.width = t_size.x;
	buttonStartSize.height = t_size.y;

	buttonSize = buttonStartSize;
	scrollHeight = t_location.y;
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

bool Button::isInside(sf::Vector2i t_click)
{
	sf::FloatRect buttonRect;
	if (isSprite)
	{
		buttonRect = m_sprite.getGlobalBounds();
	}
	else
	{
		buttonRect = m_button.getGlobalBounds();
	}
	if (t_click.x >= buttonRect.left && t_click.x < buttonRect.left + buttonRect.width &&
		t_click.y >= buttonRect.top && t_click.y <= buttonRect.top + buttonRect.height)
	{
		return true;
	}
	return false;
}

void Button::moveUp(float t_speed)
{
	sf::Vector2f pos;
	if (isSprite)
	{
		sf::Vector2f scale = m_sprite.getScale();
		pos = m_sprite.getPosition();
		scrollHeight -= t_speed;
		pos.y = scrollHeight;
		if (scrollHeight < tileListTop)
		{
			float difference = tileListTop - scrollHeight;
			if (difference <= buttonStartSize.height)
			{
				buttonSize.top = difference;
				buttonSize.height =buttonStartSize.height - difference;
			}
			sf::IntRect texRect;
			texRect.top = (buttonSize.top) / scale.x;
			texRect.left = 0;
			texRect.height = buttonSize.height / scale.x;
			texRect.width = buttonSize.width / scale.y;
			m_sprite.setTextureRect(texRect);
			pos.y = tileListTop;
		}
		else if (scrollHeight > tileListBottom - buttonStartSize.height)
		{
			float difference = buttonStartSize.height - (scrollHeight - (tileListBottom - buttonStartSize.height));
			sf::IntRect texRect;
			texRect.top = 0;
			texRect.left = 0;
			if (difference <= buttonStartSize.height)
			{
				buttonSize.height = difference;
				texRect.height = difference / scale.x;
			}
			else
			{
				texRect.height = 0;
			}
			texRect.width = buttonSize.width / scale.y;
			m_sprite.setTextureRect(texRect);
		}
		m_sprite.setPosition(pos);
	}
	else
	{
		pos = m_button.getPosition();
	}
}

void Button::moveDown(float t_speed)
{
	sf::Vector2f pos;
	if (isSprite)
	{
		sf::Vector2f scale = m_sprite.getScale();
		pos = m_sprite.getPosition();
		scrollHeight += t_speed;
		pos.y = scrollHeight;
		if (scrollHeight > tileListBottom - buttonStartSize.height)
		{
			float difference = buttonStartSize.height - (scrollHeight - (tileListBottom - buttonStartSize.height));
			sf::IntRect texRect;
			texRect.top = 0;
			texRect.left = 0;
			if (difference >= 0)
			{
				buttonSize.height = difference;
				texRect.height = difference / scale.x;
			}
			else
			{
				texRect.height = 0;
			}
			texRect.width = buttonSize.width / scale.y;
			m_sprite.setTextureRect(texRect);
		}
		else if (scrollHeight < tileListTop)
		{
			float difference = tileListTop - scrollHeight;
			if (difference <= buttonStartSize.height)
			{
				buttonSize.top = difference;
				buttonSize.height = buttonStartSize.height - difference;
			}
			else
			{
				buttonSize.top = scrollHeight;
				buttonSize.height = buttonStartSize.height;
			}
			sf::IntRect texRect;
			texRect.top = (buttonSize.top) / scale.x;
			texRect.left = 0;
			texRect.height = buttonSize.height / scale.x;
			texRect.width = buttonSize.width / scale.y;
			m_sprite.setTextureRect(texRect);
			pos.y = tileListTop;

		}
		m_sprite.setPosition(pos);
	}
	else
	{
		pos = m_button.getPosition();
	}
}
