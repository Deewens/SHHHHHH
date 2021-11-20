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

void CollisionManager::check(Player& t_player, Pickup& t_pickup)
{
	//switch (m_playerState)
	//{
	//case PlayerMovingState::RUNNING:
 //		m_canPulse = 100;
	//	break;
	//case PlayerMovingState::WALKING:
	//	m_canPulse = 20;
	//	break;
	//case PlayerMovingState::CROUCHING:
	//	m_canPulse = 5;
	//	break;
	//case PlayerMovingState::IDLE:
	//	break;
	//default:
	//	break;
	//}
	//if (m_pulseDelay >= m_canPulse)
	//{
	//	float collisionDistance = t_player.getRadius() + t_pickup.getRadius();
	//	float distanceAway = distanceBetween(t_player.getPosition(), t_pickup.getPosition());
	//	if (collisionDistance >= distanceAway)
	//	{
	//		std::cout << "pickup";
	//		Noise t_noise;
	//		t_noise.init(NoiseLevels::RED, t_player.getPosition());
	//		m_noises.push_back(t_noise);
	//		m_pulseDelay = 0;
	//	}
	//}
	//m_pulseDelay++;

	float collisionDistance = t_player.getRadius() + t_pickup.getRadius();
	float distanceAway = distanceBetween(t_player.getPosition(), t_pickup.getPosition());
	if (collisionDistance >= distanceAway)
	{
		std::cout << "pickup";
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

bool CollisionManager::collision(const sf::Sprite& object1, const sf::Sprite& object2)
{
	OrientedBoundingBox OBB1(object1);
	OrientedBoundingBox OBB2(object2);

	// Create the four distinct axes that are perpendicular to the edges of the two rectangles
	sf::Vector2f Axes[4] = {
		sf::Vector2f(OBB1.Points[1].x - OBB1.Points[0].x,
		OBB1.Points[1].y - OBB1.Points[0].y),
		sf::Vector2f(OBB1.Points[1].x - OBB1.Points[2].x,
		OBB1.Points[1].y - OBB1.Points[2].y),
		sf::Vector2f(OBB2.Points[0].x - OBB2.Points[3].x,
		OBB2.Points[0].y - OBB2.Points[3].y),
		sf::Vector2f(OBB2.Points[0].x - OBB2.Points[1].x,
		OBB2.Points[0].y - OBB2.Points[1].y)
	};

	for (int i = 0; i < 4; i++) // For each axis...
	{
		float MinOBB1, MaxOBB1, MinOBB2, MaxOBB2;

		// ... project the points of both OBBs onto the axis ...
		OBB1.ProjectOntoAxis(Axes[i], MinOBB1, MaxOBB1);
		OBB2.ProjectOntoAxis(Axes[i], MinOBB2, MaxOBB2);

		// ... and check whether the outermost projected points of both OBBs overlap.
		// If this is not the case, the Seperating Axis Theorem states that there can be no collision between the rectangles
		if (!((MinOBB2 <= MaxOBB1) && (MaxOBB2 >= MinOBB1)))
			return false;
	}

	return true;	// Collision detected!
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



