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
		m_gameState = GameState::LOSE;
	}
}

void CollisionManager::check(Player& t_player, Pickup& t_pickup, HUD& t_hud)
{

	float collisionDistance = t_player.getRadius() + t_pickup.getRadius();
	float distanceAway = distanceBetween(t_player.getPosition(), t_pickup.getPosition());
	if (collisionDistance >= distanceAway)
	{
		t_pickup.collect();
		if (!t_player.m_readyToTHrow[0])
		{
			t_player.m_readyToTHrow[0] = true;
			t_hud.m_pickUpHud[0] = true;
		}
		else if (!t_player.m_readyToTHrow[1])
		{
			t_player.m_readyToTHrow[1] = true;
			t_hud.m_pickUpHud[1] = true;
		}
	}
}

void CollisionManager::check(Player& t_player, Environment& t_environment, int& t_counter)
{
	sf::FloatRect collisionRect = t_environment.getCollisionRect();

	float playerSize = t_player.getRadius();
	sf::Vector2f playerPos = t_player.getPosition();

	float bottleSize = t_player.bottleSpriteRadius();
	sf::Vector2f bottlePos0 = t_player.bottleLocation(0);
	sf::Vector2f bottlePos1 = t_player.bottleLocation(1);


	float closestX = clamp(playerPos.x, collisionRect.left, collisionRect.left + collisionRect.width);
	float closestY = clamp(playerPos.y, collisionRect.top, collisionRect.top + collisionRect.height);


	float closestXbottle0 = clamp(bottlePos0.x, collisionRect.left, collisionRect.left + collisionRect.width);
	float closestYbottle0 = clamp(bottlePos0.y, collisionRect.top, collisionRect.top + collisionRect.height);

	float closestXbottle1 = clamp(bottlePos1.x, collisionRect.left, collisionRect.left + collisionRect.width);
	float closestYbottle1 = clamp(bottlePos1.y, collisionRect.top, collisionRect.top + collisionRect.height);


	float distanceX = playerPos.x - closestX;
	float distanceY = playerPos.y - closestY;

	float bottle0distanceX = bottlePos0.x - closestXbottle0;
	float bottle0distanceY = bottlePos0.y - closestYbottle0;

	float bottle1distanceX = bottlePos1.x - closestXbottle1;
	float bottle1distanceY = bottlePos1.y - closestYbottle1;

	float distanceSquared = (distanceX * distanceX) + (distanceY * distanceY);

	float distanceSquared0 = (bottle0distanceX * bottle0distanceX) + (bottle0distanceY * bottle0distanceY);
	float distanceSquared1 = (bottle1distanceX * bottle1distanceX) + (bottle1distanceY * bottle1distanceY);


	if (distanceSquared < (playerSize * playerSize))
	{
		if (t_environment.isImpassable())
		{
			t_player.awayFrom(sf::Vector2f(closestX, closestY));
		}
		else if (t_counter > 60)
		{
			Noise newNoise;
			newNoise.init(t_player.getNoiseLevel(), t_environment.getNoise(), playerPos);
			m_noises.push_back(newNoise);
			t_counter = 0;
		}
	}

	if (distanceSquared0 < (bottleSize * bottleSize) && t_player.m_throw[0])
	{
		if (t_environment.isImpassable())
		{
			t_player.m_bottleBreak[0] = true;
			t_player.m_particleDraw[0] = true;	
		}
	}
	if (distanceSquared1 < (bottleSize * bottleSize) && t_player.m_throw[1])
	{
		if (t_environment.isImpassable())
		{
			t_player.m_bottleBreak[1] = true;
			t_player.m_particleDraw[1] = true;

		}
	}
}


void CollisionManager::check(Player& t_player, Goal& t_goal, bool& isFound)
{
	sf::FloatRect collisionRect = t_goal.getCollisionRect();
	float playerSize = t_player.getRadius();
	sf::Vector2f playerPos = t_player.getPosition();

	float closestX = clamp(playerPos.x, collisionRect.left, collisionRect.left + collisionRect.width);
	float closestY = clamp(playerPos.y, collisionRect.top, collisionRect.top + collisionRect.height);

	float distanceX = playerPos.x - closestX;
	float distanceY = playerPos.y - closestY;

	float distanceSquared = (distanceX * distanceX) + (distanceY * distanceY);

	if (distanceSquared < (playerSize * playerSize))
	{
		m_gameState = GameState::WINLEVEL;
		isFound = true;
	}
	sf::Vector2f newNoisePos = t_player.checkBottleCollisions();
	if (newNoisePos != sf::Vector2f{})
	{
		Noise newNoise;
		newNoise.init(NoiseLevels::RED, NoiseLevels::RED, newNoisePos);
		m_noises.push_back(newNoise);
	}
}


bool CollisionManager::checkNoiseCollision(Enemy& t_enemy)
{
    // Check the collision of the enemy between the "Noise shape"
    for (Noise& noise : m_noises)
    {
        float collisionDistance = noise.getShape().getRadius() + t_enemy.getRadius();
        float distanceAway = distanceBetween(noise.getShape().getPosition(), t_enemy.getPosition());

        if (collisionDistance >= distanceAway)
        {
            t_enemy.moveTo(noise.getPosition());
            return true;
        }
    }

    return false;
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

	if (m_impact[0])
	{
		m_bottleImpact[0].Draw(t_window);
	}
	if (m_impact[1])
	{
		m_bottleImpact[1].Draw(t_window);
	}
}

void CollisionManager::update()
{
	for (size_t i = 0; i < 2; i++)
	{
		m_bottleImpact[i].Update();
	}

	for (Noise& t_noise : m_noises)
	{
		t_noise.update();
	}

    // Will check if the noise is done, thus, will erase it from the vector
	m_noises.erase(std::remove_if(m_noises.begin(), m_noises.end(), [](Noise& noise) {
		return noise.isNoiseDone();
	}), m_noises.end());
}

sf::Vector2f CollisionManager::impactLocation()
{
	return m_impactLocation;
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



