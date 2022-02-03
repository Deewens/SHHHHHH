#include "Tile.h"

Tile::Tile(Button* t_tile, int t_tileNum, sf::Texture& t_texture) : m_tileNum(t_tileNum)
{
	m_sprite.setTexture(t_texture);
	m_sprite.setTextureRect(t_tile->getTextRect());
	m_passable = t_tile->getPassable();
	m_spriteName = t_tile->getSpriteName();

	int col = t_tileNum % (screen_Width / tileSize);
	int row = (t_tileNum - col) / (screen_Width / tileSize);

	m_sprite.setPosition((col + 0.5) * tileSize, (row + 0.5) * tileSize);
	m_sprite.setOrigin(m_sprite.getGlobalBounds().width / 2, m_sprite.getGlobalBounds().height / 2);
	m_sprite.setScale(tileSize / m_sprite.getGlobalBounds().width, tileSize / m_sprite.getGlobalBounds().height);
}

void Tile::render(sf::RenderWindow& t_window)
{
	t_window.draw(m_sprite);
}

void Tile::rotate()
{
	int rotation = m_sprite.getRotation();
	rotation += 90;
	m_sprite.setRotation(rotation);
}

std::string Tile::getJsonInfo(int t_gridIndex)
{
	std::string impassable;
	if (!m_passable)
	{
		impassable = "true";
	}
	else
	{
		impassable = "false";
	}
	std::string output = "\n		{\n			\"gridIndex\": " + std::to_string(t_gridIndex) +
		",\n			\"impassable\": " + impassable + 
		",\n			\"rotation\": " + std::to_string(m_sprite.getRotation()) + 
		",\n			\"spriteName\": \"" + m_spriteName + "\"\n		},";
	return output;
}
