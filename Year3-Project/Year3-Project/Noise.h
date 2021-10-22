#pragma once
#include <SFML/Graphics.hpp>
#include "Globals.h"

class Noise
{
public:

	Noise(int t_strength , sf::Vector2f t_location);

	~Noise();

	void render();

	void update();

private:
	sf::CircleShape m_noiseShape;
	float m_maxStrength{0.0f};
	float m_strength{0.0f};
};

