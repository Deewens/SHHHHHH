#pragma once
//#include <iostream>
#include "Player.h"
#include "Enemy.h"
#include "Pickup.h"
#include "Noise.h"
#include <ctime>

class CollisionManager
{
public:
	CollisionManager();
	void check(Player& t_player, Enemy& t_enemy);
	void check(Player& t_player, Pickup& t_pickup);
	void renderNoises(sf::RenderWindow& t_window);
	void update();
    std::vector<Noise>* getNoises();
private:
	int pulseDelay;
	const int CAN_PULSE = 30;
	float distanceBetween(sf::Vector2f t_a, sf::Vector2f t_b);
	std::vector<Noise> m_noises;
};