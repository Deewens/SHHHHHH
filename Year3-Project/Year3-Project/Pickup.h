#pragma once
#include <SFML/Graphics.hpp>
#include <iostream>

class Pickup : public sf::Drawable
{
public:
	Pickup();

	sf::Vector2f getPosition();
	float getRadius();
private:
	sf::RectangleShape rect;

    void draw(sf::RenderTarget &target, sf::RenderStates states) const final;
};

