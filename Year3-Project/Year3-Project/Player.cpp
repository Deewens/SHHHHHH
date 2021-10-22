#include "Player.h"

Player::Player()
{
	m_speed = 2;
	Player::loadImage();

	m_sprite.setOrigin(19, 22);
	m_sprite.setPosition(100, 100);
	m_sprite.setTextureRect(sf::IntRect(351, 132, 38, 44));
	
}



void Player::setDirection(int t_direction)
{
	m_direction = t_direction;
	if (t_direction == NORTH)
	{
		m_sprite.setRotation(270);
		m_sprite.move(0, -m_speed);
	}
	else if (t_direction == SOUTH)
	{
		m_sprite.setRotation(90);
		m_sprite.move(0, m_speed);
	}
	else if (t_direction == EAST)
	{
		m_sprite.setRotation(0);
		m_sprite.move(m_speed, 0);

	}
	else if (t_direction == WEST)
	{
		m_sprite.setRotation(180);
		m_sprite.move(-m_speed, 0);

	}
	else if (t_direction == NORTHWEST)
	{
		m_sprite.setRotation(225);
		m_sprite.move(-m_speed, -m_speed);
	}
	else if (t_direction == NORTHEAST)
	{
		m_sprite.setRotation(315);
		m_sprite.move(m_speed, -m_speed);
	}
	else if (t_direction == SOUTHWEST)
	{
		m_sprite.setRotation(135);
		m_sprite.move(-m_speed, m_speed);
	}
	else if (t_direction == SOUTHEAST)
	{
		m_sprite.setRotation(45);
		m_sprite.move(m_speed, m_speed);
	}
}

void Player::update()
{
	move();
}

void Player::move()
{
	if (!sf::Keyboard::isKeyPressed(sf::Keyboard::Up) && !sf::Keyboard::isKeyPressed(sf::Keyboard::Down) &&
		!sf::Keyboard::isKeyPressed(sf::Keyboard::Left) && !sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
	{
		m_isMoving = false;
	}
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up) && sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
	{
		m_isMoving = true;
		setDirection(NORTHWEST);
	}
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up) && sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
	{
		m_isMoving = true;
		setDirection(NORTHEAST);
	}
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down) && sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
	{
		m_isMoving = true;
		setDirection(SOUTHWEST);
	}
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down) && sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
	{
		m_isMoving = true;
		setDirection(SOUTHEAST);
	}	
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
	{
		m_isMoving = true;
		setDirection(NORTH);
	}
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
	{
		m_isMoving = true;
		setDirection(SOUTH);
	}
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
	{
		m_isMoving = true;
		setDirection(WEST);
	}
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
	{
		m_isMoving = true;
		setDirection(EAST);
	}
	

	
}

void Player::render(sf::RenderWindow& t_window)
{
	t_window.draw(m_sprite);
}
