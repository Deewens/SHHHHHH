#pragma once

#include <SFML/Graphics.hpp>
#include <iostream>
#include"Globals.h"

class Player
{

private:

	sf::Sprite m_playerSprite;

	sf::Texture m_playerTexture;

	float m_speed{};

	bool m_isAlive=true; 

	int m_direction; 

	bool m_isMoving=false;

public:

	Player();

	void loadImage();

	void setDirection(int t_direction);

	void update();

	void move();

	void render(sf::RenderWindow& t_window);

};

