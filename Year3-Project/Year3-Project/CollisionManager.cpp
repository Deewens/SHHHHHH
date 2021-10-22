#include "CollisionManager.h"

CollisionManager::CollisionManager()
{
}

void CollisionManager::check(Player& t_player, Enemy& t_enemy)
{
	float collisionDistance = t_player.getRadius() + t_enemy.getRadius();
	float distanceAway = distanceBetween(t_player.getPosition(), t_enemy.getPosition());
	if (collisionDistance >= distanceAway)
	{
		std::cout << "collide";
	}
}

void CollisionManager::check(Player& t_player, Pickup& t_pickup)
{
	float collisionDistance = t_player.getRadius() + t_pickup.getRadius();
	float distanceAway = distanceBetween(t_player.getPosition(), t_pickup.getPosition());
	if (collisionDistance >= distanceAway)
	{
		std::cout << "pickup";
	}
}

float CollisionManager::distanceBetween(sf::Vector2f t_a, sf::Vector2f t_b)
{
	sf::Vector2f vectorBetween = t_a - t_b;
	return sqrt((vectorBetween.x * vectorBetween.x) + (vectorBetween.y * vectorBetween.y));
}
