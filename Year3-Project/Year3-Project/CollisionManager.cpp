#include "CollisionManager.h"

CollisionManager::CollisionManager()
{
	m_pulseDelay = 0;
	m_canPulse = 0;
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

bool CollisionManager::check(Player& t_player, Pickup& t_pickup)
{

	float collisionDistance = t_player.getRadius() + t_pickup.getRadius();
	float distanceAway = distanceBetween(t_player.getPosition(), t_pickup.getPosition());
	if (collisionDistance >= distanceAway)
	{
		return true;
	}
	else
	{
		return false;
	}
}

void CollisionManager::check(Player& t_player, Environment& t_environment)
{
	sf::FloatRect collisionRect = t_environment.getCollisionRect();
	float playerSize = t_player.getRadius();
	sf::Vector2f playerPos = t_player.getPosition();
	float closestX = clamp(playerPos.x, collisionRect.left, collisionRect.left + collisionRect.width);
	float closestY = clamp(playerPos.y, collisionRect.top, collisionRect.top + collisionRect.height);

	float distanceX = playerPos.x - closestX;
	float distanceY = playerPos.y - closestY;

	float distanceSquared = (distanceX * distanceX) + (distanceY * distanceY);
	if(distanceSquared < (playerSize * playerSize))
	{
		t_player.awayFrom(sf::Vector2f(closestX, closestY));
	}
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

float CollisionManager::clamp(float t_pos, float t_min, float t_max)
{
	if (t_pos < t_min) t_pos = t_min;
	if (t_pos > t_max) t_pos = t_max;
	return t_pos;
}



