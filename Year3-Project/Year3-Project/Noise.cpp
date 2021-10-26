#include "Noise.h"

Noise::Noise()
{
	m_strength = 0;
}
Noise::~Noise()
{
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
		m_maxStrength = 10;
		break;
	case NoiseLevels::YELLOW:
		m_maxStrength = 20;
		break;
	case NoiseLevels::RED:
		m_maxStrength = 30;
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
		
	if (m_strength<= m_maxStrength)
	{
		m_strength = m_strength + RADIOUS_SCALE;
		m_noiseShape.setOrigin(sf::Vector2f(m_strength, m_strength));
		m_noiseShape.setRadius(m_strength);
	}
	else
	{
		m_strength = 0;
	}
	if (m_strength < 10)
	{
		m_noiseShape.setFillColor(sf::Color::Green);
	}
	else if (m_strength <=20)
	{
		m_noiseShape.setFillColor(sf::Color::Yellow);
	}
	else
	{
		m_noiseShape.setFillColor(sf::Color::Red);
	}

}
