#pragma once
#include <SFML/Graphics.hpp>
#include "Globals.h"

class Noise
{
public:
	Noise();
	~Noise();
	void init(NoiseLevels t_noiseLevel, sf::Vector2f t_location);
	void render(sf::RenderWindow & t_window);
	void update();

private:
	sf::CircleShape m_noiseShape;
	float m_maxStrength{0.0f};
	float m_strength{0.0f};
	const float RADIUS_SCALE{ 0.5f };
};

