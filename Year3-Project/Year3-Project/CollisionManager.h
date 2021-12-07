#pragma once
//#include <iostream>
#include "Player.h"
#include "Enemy.h"
#include "Pickup.h"
#include "Noise.h"
#include "Environment.h"
#include <ctime>

class CollisionManager
{
public:

	CollisionManager();
	void check(Player& t_player, Enemy& t_enemy);
	void check(Player& t_player, Pickup& t_pickup);
	void check(Player& t_player, Environment& t_environment);


	void renderNoises(sf::RenderWindow& t_window);
	void update();
private:

	int m_pulseDelay;

	int m_canPulse;

	float distanceBetween(sf::Vector2f t_a, sf::Vector2f t_b);
	float clamp(float t_pos, float t_min, float t_max);
	std::vector<Noise> m_noises;
};