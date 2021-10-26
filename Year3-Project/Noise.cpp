#include "Noise.h"

Noise::Noise(int t_strength , sf::Vector2f t_location)
{

	t_strength = m_maxStrength;
	m_noiseShape.setPosition(t_location);
	m_noiseShape.setRadius(m_strength);
	
}

Noise::~Noise()
{
}

void Noise::render()
{
}

void Noise::update()
{	
	if (m_strength<= m_maxStrength)
	{
		m_strength = m_strength + 0.01;
		m_noiseShape.setOrigin(sf::Vector2f(m_strength, m_strength));
		m_noiseShape.setRadius(m_strength);
	}
}
