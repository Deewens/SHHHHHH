#pragma once
#include <SFML/Graphics.hpp>
#include "Globals.h"

class Noise
{
public:
	Noise();
	void init(NoiseLevels t_noiseLevel, sf::Vector2f t_location);
	void render(sf::RenderWindow & t_window);
	void update();

    bool isNoiseDone();

private:
	sf::CircleShape m_noiseShape;
	float m_maxStrength{0.0f};
	float m_strength{0.0f};
    float RADIUS_SCALE{ 0.5f };
    bool m_noiseDone;

};

