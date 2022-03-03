#include "Pickup.h"


Pickup::Pickup(int t_gridNum, sf::Texture& t_texture)
{	
	m_gridNum = t_gridNum;
   setUpSpriteBottle(t_texture);	
}

void Pickup::setUpSpriteBottle(sf::Texture& t_texture)
{
	std::ifstream spriteData("ASSETS/TileSheet/spritesheet.json");
	nlohmann::json json;
	spriteData >> json;

	nlohmann::json frame = json["frames"]["Bottle.png"]["frame"];
	
	int x = frame["x"];
	int y = frame["y"];
	float width = frame["w"];
	float height = frame["h"];

	m_bottleSprite.setTexture(t_texture);
	//m_bottleSprite.setScale(0.05, 0.05);
	m_bottleSprite.setTextureRect(sf::IntRect(x, y, width, height));
	m_bottleSprite.setOrigin(width / 2, height / 2);
	m_bottleSprite.setScale(20 / width, 50 / height);

	m_col = m_gridNum % (screen_Width / tileSize);
	m_row= (m_gridNum - m_col) / (screen_Width / tileSize);

	m_position.x = (m_col * tileSize) + (tileSize / 2);
	m_position.y = (m_row * tileSize) + (tileSize / 2);

	m_bottleSprite.setPosition(m_position);
	
}


sf::Sprite Pickup::getSprite()
{	
	return m_bottleSprite;	
}

sf::Vector2f Pickup::getPosition()
{
	return m_bottleSprite.getPosition();
}

float Pickup::getRadius()
{
	return m_bottleSprite.getOrigin().x;
}

//void Pickup::throwPickUp(float t_angle, sf::Vector2f t_position , float t_power)
//{
//	m_bottleVelocity = sf::Vector2f(t_power * cos(PI * t_angle / 180), t_power * sin(PI * t_angle / 180)); // changes velocity (sin needs to be negative due to y axis being at top)
//}

void Pickup::draw(sf::RenderTarget &target, sf::RenderStates states) const
{	
	target.draw(m_bottleSprite);	
}


