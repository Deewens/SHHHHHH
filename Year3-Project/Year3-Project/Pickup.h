#pragma once
#include <SFML/Graphics.hpp>
#include <iostream>
#include "Globals.h"

class Pickup : public sf::Drawable
{
public:
	Pickup(int t_gridNum);

	void setUpSpriteBottle();

	sf::Sprite getSprite();

	//bool pickUpCollected(sf::Sprite m_player);

	sf::Vector2f Pickup::getPosition();

	float Pickup::getRadius();

private:

	sf::Texture m_bottleTexture;
	sf::Sprite m_bottleSprite;

    void draw(sf::RenderTarget &target, sf::RenderStates states) const final;

	int m_gridNum = -1;

	float m_col{};
	float m_row{};

	sf::Vector2f m_position{};
};

