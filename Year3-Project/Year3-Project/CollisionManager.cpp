#include "CollisionManager.h"

CollisionManager::CollisionManager()
{
	pulseDelay = 0;
}

void CollisionManager::check(Player& t_player, Enemy& t_enemy)
{
	float collisionDistance = t_player.getRadius() + t_enemy.getRadius();
	float distanceAway = distanceBetween(t_player.getPosition(), t_enemy.getPosition());
	t_enemy.visionConeCollisionCheck(t_player.getPosition());
	if (collisionDistance >= distanceAway)
	{
		std::cout << "collide";
	}
}

void CollisionManager::check(Player& t_player, Pickup& t_pickup)
{
	if (pulseDelay >= CAN_PULSE)
	{
		float collisionDistance = t_player.getRadius() + t_pickup.getRadius();
		float distanceAway = distanceBetween(t_player.getPosition(), t_pickup.getPosition());
		if (collisionDistance >= distanceAway)
		{
			std::cout << "pickup";
			Noise t_noise;
			t_noise.init(NoiseLevels::RED, t_player.getPosition());
			m_noises.push_back(t_noise);
			pulseDelay = 0;
		}
	}
	pulseDelay++;
}

void CollisionManager::renderNoises(sf::RenderWindow& t_window)
{
	for (Noise& t_noise: m_noises)
	{
        if (!t_noise.isNoiseDone())
        {
            t_noise.render(t_window);
        }
	}
}

void CollisionManager::update()
{
	for (Noise& t_noise : m_noises)
	{
		t_noise.update();
	}
}

float CollisionManager::distanceBetween(sf::Vector2f t_a, sf::Vector2f t_b)
{
	sf::Vector2f vectorBetween = t_a - t_b;
	return sqrt((vectorBetween.x * vectorBetween.x) + (vectorBetween.y * vectorBetween.y));
}

std::vector<Noise>* CollisionManager::getNoises()
{
    return &m_noises;
}
