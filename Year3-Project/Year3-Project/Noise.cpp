#include "Noise.h"

Noise::Noise()
{
	m_strength = 0;
	m_noiseShape.setOutlineThickness(1.0f);
	m_noiseShape.setFillColor(sf::Color::Transparent);
    m_noiseDone = false;
}

void Noise::init(NoiseLevels t_noiseLevel, sf::Vector2f t_location)
{
	m_noiseLevel = t_noiseLevel;

	switch (m_noiseLevel)
	{
	case NoiseLevels::WHITE:
		m_maxStrength = 0;
		break;
	case NoiseLevels::GREEN:
		m_noiseShape.setOutlineColor(sf::Color::Green);
		m_maxStrength = 50;
		break;
	case NoiseLevels::YELLOW:
		m_noiseShape.setOutlineColor(sf::Color::Yellow);
		m_maxStrength = 100;
		break;
	case NoiseLevels::RED:
		m_noiseShape.setOutlineColor(sf::Color::Red);
		m_maxStrength = 150;
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
