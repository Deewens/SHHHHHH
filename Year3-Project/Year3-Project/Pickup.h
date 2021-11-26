#pragma once
#include <SFML/Graphics.hpp>
#include <iostream>
#include "Globals.h"

class Pickup : public sf::Drawable
{
public:
	Pickup();
	Pickup(int t_gridNum , const PickUpType& m_type);

	sf::Vector2f getPosition();

	float getRadius();

	void setUpSpriteBottle();

	void setUpSpriteCan();
private:

	sf::RectangleShape rect;

	sf::Texture m_bottleTexture;
	sf::Sprite m_bottleSprite;

	sf::Texture m_canTexture;
	sf::Sprite m_canSprite;


    void draw(sf::RenderTarget &target, sf::RenderStates states) const final;


};

