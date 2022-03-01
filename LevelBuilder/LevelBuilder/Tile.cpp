#include "Tile.h"

Tile::Tile(Button* t_tile, int t_tileNum, sf::Texture& t_texture) : m_tileNum(t_tileNum)
{
	m_objectSprite.setTexture(t_texture);
	m_groundSprite.setTexture(t_texture);
	int col = t_tileNum % (screen_Width / tileSize);
	int row = (t_tileNum - col) / (screen_Width / tileSize);
	m_objectSprite.setPosition((col + 0.5) * tileSize, (row + 0.5) * tileSize);
	m_groundSprite.setPosition((col + 0.5) * tileSize, (row + 0.5) * tileSize);

	if (t_tile->getPassable())
	{
		m_groundSprite.setTextureRect(t_tile->getTextRect());
		m_groundSpriteName = t_tile->getSpriteName();
		m_groundActive = true;
		m_groundSprite.setOrigin(m_groundSprite.getGlobalBounds().width / 2, m_groundSprite.getGlobalBounds().height / 2);
		m_groundSprite.setScale(tileSize / m_groundSprite.getGlobalBounds().width, tileSize / m_groundSprite.getGlobalBounds().height);
	}
	else
	{
		m_objectSprite.setTextureRect(t_tile->getTextRect());
		m_objectSpriteName = t_tile->getSpriteName();
		m_objectActive = true;
		m_objectSprite.setOrigin(m_objectSprite.getGlobalBounds().width / 2, m_objectSprite.getGlobalBounds().height / 2);
		m_objectSprite.setScale(tileSize / m_objectSprite.getGlobalBounds().width, tileSize / m_objectSprite.getGlobalBounds().height);
		specialHere = t_tile->getSpecial();
	}
}

void Tile::changeLayers(Button* t_tile)
{
	if (t_tile->getPassable())
	{
		m_groundSprite.setTextureRect(t_tile->getTextRect());
		m_groundSpriteName = t_tile->getSpriteName();
		m_groundActive = true;
		m_groundSprite.setOrigin(m_groundSprite.getGlobalBounds().width / 2, m_groundSprite.getGlobalBounds().height / 2);
		m_groundSprite.setScale(tileSize / m_groundSprite.getGlobalBounds().width, tileSize / m_groundSprite.getGlobalBounds().height);
	}
	else
	{
		m_objectSprite.setTextureRect(t_tile->getTextRect());
		m_objectSpriteName = t_tile->getSpriteName();
		m_objectActive = true;
		m_objectSprite.setOrigin(m_objectSprite.getGlobalBounds().width / 2, m_objectSprite.getGlobalBounds().height / 2);
		m_objectSprite.setScale(tileSize / m_objectSprite.getGlobalBounds().width, tileSize / m_objectSprite.getGlobalBounds().height);
		specialHere = t_tile->getSpecial();
	}
}

void Tile::render(sf::RenderWindow& t_window)
{
	if (m_groundActive)
	{
		t_window.draw(m_groundSprite);
	}
	if (m_objectActive)
	{
		t_window.draw(m_objectSprite);
	}
}

void Tile::rotate()
{
	int rotation;
	if (m_objectActive)
	{
		rotation = m_objectSprite.getRotation();
		rotation += 90;
		m_objectSprite.setRotation(rotation);
	}
	if (m_groundActive)
	{
		rotation = m_groundSprite.getRotation();
		rotation += 90;
		m_groundSprite.setRotation(rotation);
	}
}

std::string Tile::getJsonInfo(int t_gridIndex)
{
	std::string output = "";
	if (m_groundActive)
	{
		output += "\n		{\n			\"gridIndex\": " + std::to_string(t_gridIndex) +
			",\n			\"impassable\": false" +
			",\n			\"rotation\": " + std::to_string(m_groundSprite.getRotation()) +
			",\n			\"spriteName\": \"" + m_groundSpriteName + "\"\n		},";
	}
	if (m_objectActive && !specialHere)
	{
		output += "\n		{\n			\"gridIndex\": " + std::to_string(t_gridIndex) +
			",\n			\"impassable\": true" +
			",\n			\"rotation\": " + std::to_string(m_objectSprite.getRotation()) +
			",\n			\"spriteName\": \"" + m_objectSpriteName + "\"\n		},";
	}
	return output;
}

std::string Tile::getSpecialJson(int t_gridIndex)
{
	
	std::string output = "";
	if (specialHere)
	{
		std::string type = "error";
		if (m_objectSpriteName == "Idle_1.png")
		{
			type = "Player";
		}
		else if(m_objectSpriteName == "Bottle.png")
		{
			type = "Pickup";
		}
		else if (m_objectSpriteName == "green_zombie_idling_1.png")
		{
			type = "Zombie";
		}
		output += "\n		{\n			\"gridIndex\": " + std::to_string(t_gridIndex) +
			//",\n			\"impassable\": true" +
			",\n			\"rotation\": " + std::to_string(m_objectSprite.getRotation()) +
			",\n			\"Type\": \"" + type + "\"\n		},";
	}
	return output;
}
