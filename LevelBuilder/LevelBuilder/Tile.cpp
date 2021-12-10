#include "Tile.h"

Tile::Tile(Button* t_tile, int t_tileNum) : m_tileNum(t_tileNum)
{
	m_sprite.setTexture(t_tile->getTexture());
	m_passable = false;
	m_spriteName = t_tile->getSpriteName();

	int col = t_tileNum % (screen_Width / tileSize);
	int row = (t_tileNum - col) / (screen_Width / tileSize);

	m_sprite.setPosition(col * tileSize, row * tileSize);
	m_sprite.setScale(tileSize / m_sprite.getGlobalBounds().width, tileSize / m_sprite.getGlobalBounds().height);
}

void Tile::render(sf::RenderWindow& t_window)
{
	t_window.draw(m_sprite);
}
