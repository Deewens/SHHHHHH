#pragma once

#include <SFML/Graphics.hpp>
#include <iostream>
#include "Globals.h"
#include "Character.h"

class Player : public Character
{

private:
	bool m_isAlive=true;

public:

	Player();

	void setDirection(int t_direction);

	void update() override;

	void move();

	void render(sf::RenderWindow& t_window) override;

};

