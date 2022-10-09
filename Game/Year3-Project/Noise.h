#pragma once
#include <SFML/Graphics.hpp>
#include "Globals.h"

class Noise
{
public:
	Noise();
	void init(NoiseLevels t_playerNoiseLevel, NoiseLevels t_envNoiseLevel, sf::Vector2f t_location);
	void render(sf::RenderWindow & t_window);
	void update();

    sf::Vector2f getPosition();

    bool isNoiseDone();

    sf::CircleShape getShape();

private:
    NoiseLevels m_noiseLevel;
	sf::CircleShape m_noiseShape;
	float m_maxStrength{0.0f};
	float m_strength{0.0f};
    float RADIUS_SCALE{ 0.5f };

    bool m_noiseDone;
};

