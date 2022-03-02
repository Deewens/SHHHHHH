#pragma once

#include <SFML/Graphics.hpp>
#include "Globals.h"

class Particle
{

public:

	int timetoLive;
	sf::Vector2f velocity;
	sf::RectangleShape shape;

	void Draw(sf::RenderWindow& win)
	{
		if (timetoLive > 0)
		{
			win.draw(shape);
		}
	}
	bool Update()
	{
		if (timetoLive > 0)
		{
			shape.move(velocity);

			timetoLive--;
		}
		return timetoLive > 0;
	}

	

	Particle() {}

	Particle(sf::Vector2f pos, sf::Vector2f vel, sf::Color t_color)
	{
		shape.setSize(sf::Vector2f(6, 6));
		shape.setPosition(pos);
		shape.setFillColor(t_color);

		velocity = vel;
		//timetoLive = 200;
		timetoLive = rand() % 40;
	}
};


class ParticleSystem
{

public:

	Particle particles[MAX_PARTICLES];
	sf::Vector2f position;

	void Initialise(sf::Vector2f pos , sf::Color t_color)
	{
		if (ready)
		{
			position = pos;
			for (int i = 0; i < MAX_PARTICLES; i++)
			{
				particles[i] = Particle(position, sf::Vector2f(rand() / double(RAND_MAX) * 4 - 2, rand() / double(RAND_MAX) * 4 - 2), t_color);
			}
			ready = false;
		}
	}
	void Update()
	{
		bool isDrawing = false;
		for (int i = 0; i < MAX_PARTICLES; i++)
		{
			if (particles[i].Update())
			{
				isDrawing = true;
			}
		}
		if (!isDrawing)
		{
			ready = true;
		}
	}
	void Draw(sf::RenderWindow& win)
	{
		for (int i = 0; i < MAX_PARTICLES; i++)
		{
			particles[i].Draw(win);
		}
	}
	ParticleSystem() {}
private:
	int ready;
};



