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
	if (t_direction == SOUTH)
	{
		m_sprite.setRotation(90);
		m_sprite.move(0, m_speed);
	}
	if (t_direction == EAST)
	{
		m_sprite.setRotation(0);
		m_sprite.move(m_speed, 0);

	}
	if (t_direction == WEST)
	{
		m_sprite.setRotation(180);
		m_sprite.move(-m_speed, 0);

	}
	if (t_direction == NORTHWEST)
	{
		m_sprite.setRotation(225);
	}
	if (t_direction == NORTHEAST)
	{
		m_sprite.setRotation(315);
	}
	if (t_direction == SOUTHWEST)
	{
		m_sprite.setRotation(135);
	}
	if (t_direction == SOUTHEAST)
	{
		m_sprite.setRotation(45);
	}
}

void Player::update()
{
	move();
}

void Player::move()
{
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
	{
		m_isMoving = true;
		setDirection(NORTH);
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
	{
		m_isMoving = true;
		setDirection(SOUTH);
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
	{
		m_isMoving = true;
		setDirection(WEST);
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
	{
		m_isMoving = true;
		setDirection(EAST);
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up) && sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
	{
		m_isMoving = true;
		setDirection(NORTHWEST);
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up) && sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
	{
		m_isMoving = true;
		setDirection(NORTHEAST);
	}

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down) && sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
	{
		m_isMoving = true;
		setDirection(SOUTHWEST);
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down) && sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
	{
		m_isMoving = true;
		setDirection(SOUTHEAST);
	}

	if (!sf::Keyboard::isKeyPressed(sf::Keyboard::Up) && !sf::Keyboard::isKeyPressed(sf::Keyboard::Down) &&
		!sf::Keyboard::isKeyPressed(sf::Keyboard::Left) && !sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
	{
		m_isMoving = false;
	}	
}

void Player::render(sf::RenderWindow& t_window)
{
	t_window.draw(m_sprite);
}
