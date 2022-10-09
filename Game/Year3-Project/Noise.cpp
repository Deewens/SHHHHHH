#include "Noise.h"

#include <iostream>

Noise::Noise()
{
	m_strength = 0;
	m_noiseShape.setOutlineThickness(1.0f);
	m_noiseShape.setFillColor(sf::Color::Transparent);
    m_noiseDone = false;
}

void Noise::init(NoiseLevels t_playerNoiseLevel, NoiseLevels t_envNoiseLevel, sf::Vector2f t_location)
{
    int result = static_cast<int>(t_envNoiseLevel) * static_cast<int>(t_playerNoiseLevel);

    if (result > static_cast<int>(NoiseLevels::RED)) m_noiseLevel = NoiseLevels::RED;
    else m_noiseLevel = static_cast<NoiseLevels>(result);

	switch (m_noiseLevel)
	{
	case NoiseLevels::WHITE:
		m_maxStrength = 0;
		break;
	case NoiseLevels::GREEN:
		m_noiseShape.setOutlineColor(sf::Color(0, 255, 0, 128));
		m_noiseShape.setOutlineThickness(4);
		m_maxStrength = 100;
		break;
	case NoiseLevels::YELLOW:
		m_noiseShape.setOutlineColor(sf::Color(255, 255, 0, 128));
		m_noiseShape.setOutlineThickness(4);
		m_maxStrength = 200;
		break;
	case NoiseLevels::RED:
		m_noiseShape.setOutlineColor(sf::Color(255, 0, 0, 128));
		m_noiseShape.setOutlineThickness(4);
		m_maxStrength = 300;
	default:
		break;
	}

	m_noiseShape.setPosition(t_location);

	m_noiseShape.setRadius(m_strength);
}

void Noise::render(sf::RenderWindow& t_window)
{
	t_window.draw(m_noiseShape);
}

void Noise::update()
{	
		
	if (m_strength <= m_maxStrength)
	{
		m_strength = m_strength + RADIUS_SCALE;
		m_noiseShape.setOrigin(sf::Vector2f(m_strength, m_strength));
		m_noiseShape.setRadius(m_strength);
	}
	else
	{
        m_noiseDone = true;
	}
}

bool Noise::isNoiseDone()
{
    return this->m_noiseDone;
}

sf::CircleShape Noise::getShape()
{
    return m_noiseShape;
}

sf::Vector2f Noise::getPosition()
{
    return m_noiseShape.getPosition();
}
