#pragma once
//#include <iostream>
#include "Player.h"
#include "Enemy.h"
#include "Pickup.h"
#include "Noise.h"

class CollisionManager
{
public:
	CollisionManager();
	void check(Player& t_player, Enemy& t_enemy);
	void check(Player& t_player, Pickup& t_pickup);
	void renderNoises(sf::RenderWindow& t_window);
	void update();
private:

	float distanceBetween(sf::Vector2f t_a, sf::Vector2f t_b);

	Noise m_noise;
};