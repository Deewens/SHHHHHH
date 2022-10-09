#pragma once
//#include <iostream>
#include "Player.h"
#include "Enemy.h"
#include "Pickup.h"
#include "Noise.h"
#include "Environment.h"
#include <ctime>
#include "ParticleSystem.h"
#include "Goal.h"
#include "HUD.h"
#include <functional>

class CollisionManager
{
public:

	CollisionManager();
	void check(Player& t_player, Enemy& t_enemy);
	void check(Player& t_player, Pickup& t_pickup, HUD& t_hud);
	void check(Player& t_player, Environment& t_environment, int& t_counter);
	void check(Player& t_player, Goal& t_goal, bool& isFound);
    bool checkNoiseCollision(Enemy& t_enemy);

	void renderNoises(sf::RenderWindow& t_window);
	void update();

	void clearNoises();

	sf::Vector2f impactLocation();

	sf::Vector2f m_impactLocation;

	ParticleSystem m_bottleImpact[2];

	bool m_impact[2]={ false,false };

private:

	int m_pulseDelay;

	int m_canPulse;

	float distanceBetween(sf::Vector2f t_a, sf::Vector2f t_b);
	float clamp(float t_pos, float t_min, float t_max);
	std::vector<Noise> m_noises;

	sf::Color m_color = sf::Color(210, 105, 30);


};