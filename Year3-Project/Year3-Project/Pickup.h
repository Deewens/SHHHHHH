#pragma once
#include <SFML/Graphics.hpp>
#include <iostream>

class Pickup : public sf::Drawable
{
public:
	Pickup();

	sf::Vector2f getPosition();
	float getRadius();

	void setUpSprite();
private:

	sf::RectangleShape rect;

	sf::Texture m_bottleTexture;
	sf::Sprite m_bottleSprite;

	sf::Texture m_canTexture;
	sf::Sprite m_canSprite;


    void draw(sf::RenderTarget &target, sf::RenderStates states) const final;


};

