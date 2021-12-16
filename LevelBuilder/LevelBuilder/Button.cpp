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
	
	m_buttonColor = t_color;
}

Button::Button(sf::Vector2f t_location, sf::Vector2f t_size, sf::Texture t_texture, std::string t_spriteName) : m_texture(t_texture), m_spriteName(t_spriteName)
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
	checkSpriteVisible();

	m_button.setFillColor(sf::Color::Transparent);
	m_button.setOutlineThickness(5);
	m_button.setOutlineColor(sf::Color::Red);
}

void Button::render(sf::RenderWindow& t_window)
{
	if (isSprite)
	{
		t_window.draw(m_sprite);
		if (isSelected && scrollHeight > tileListTop - buttonStartSize.height && scrollHeight < tileListBottom)
		{
			t_window.draw(m_button);
		}
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
		if (t_click.x >= buttonRect.left && t_click.x < buttonRect.left + buttonRect.width &&
			t_click.y >= buttonRect.top && t_click.y <= buttonRect.top + buttonRect.height)
		{
			return true;
		}
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
	if (isSprite)
	{
		scrollHeight -= t_speed;
		checkSpriteVisible();
	}
}

void Button::moveDown(float t_speed)
{
	if (isSprite)
	{
		scrollHeight += t_speed;
		checkSpriteVisible();
	}
}

void Button::setSelected(bool t_selected)
{
	isSelected = t_selected;
	if (isSprite && isSelected)
	{
		m_button.setPosition(m_sprite.getPosition());
		m_button.setSize(sf::Vector2f(m_sprite.getGlobalBounds().width, m_sprite.getGlobalBounds().height));
	}
	if (!isSprite)
	{
		if (isSelected)
		{
			m_button.setFillColor(sf::Color::Yellow);
		}
		else
		{
			m_button.setFillColor(m_buttonColor);
		}
	}
}

sf::Texture& Button::getTexture()
{
	return m_texture;
}

std::string Button::getSpriteName()
{
	return m_spriteName;
}

void Button::checkSpriteVisible()
{
	sf::Vector2f pos;
	sf::Vector2f scale = m_sprite.getScale();
	pos = m_sprite.getPosition();
	sf::IntRect texRect;
	pos.y = scrollHeight;
	if (scrollHeight < tileListTop)
	{
		float difference = tileListTop - scrollHeight;
		if (difference <= buttonStartSize.height)
		{
			buttonSize.top = difference;
			buttonSize.height = buttonStartSize.height - difference;
		}
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
		texRect.top = 0;
		texRect.left = 0;
		if (difference <= buttonStartSize.height && difference >= 0)
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
	if (isSelected)
	{
		m_button.setPosition(pos);
		m_button.setSize(sf::Vector2f(m_sprite.getGlobalBounds().width, m_sprite.getGlobalBounds().height));
	}
}
