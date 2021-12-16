#include "Pickup.h"


Pickup::Pickup(int t_gridNum)
{	
	m_gridNum = t_gridNum;
   setUpSpriteBottle();	
}

void Pickup::setUpSpriteBottle()
{
	if (!m_bottleTexture.loadFromFile("ASSETS\\IMAGES\\Bottle.png"))
	{
		// simple error message if previous call fails
		std::cout << "problem loading backpack texture" << std::endl;
	}

	m_bottleSprite.setTexture(m_bottleTexture);
	m_bottleSprite.setScale(0.025, 0.025);		

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

void Pickup::throwPickUp(float t_angle, sf::Vector2f t_position , float t_power)
{
	m_bottleVelocity = sf::Vector2f(t_power * cos(PI * t_angle / 180), t_power * sin(PI * t_angle / 180)); // changes velocity (sin needs to be negative due to y axis being at top)
}

void Pickup::draw(sf::RenderTarget &target, sf::RenderStates states) const
{	
	target.draw(m_bottleSprite);	
}

void Pickup::move()
{
	m_bottleSprite.setPosition(m_bottleSprite.getPosition() + m_bottleVelocity);
}


