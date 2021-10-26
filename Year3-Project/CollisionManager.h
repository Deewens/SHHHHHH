#pragma once
//#include <iostream>
#include "Player.h"
#include "Enemy.h"
#include "Pickup.h"
class CollisionManager
{
public:
	CollisionManager();
	void check(Player& t_player, Enemy& t_enemy);
	void check(Player& t_player, Pickup& t_pickup);

private:
	float distanceBetween(sf::Vector2f t_a, sf::Vector2f t_b);
};