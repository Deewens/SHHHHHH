/// <summary>
/// author Pete Lowe May 2019
/// you need to change the above line or lose marks
/// </summary>
#ifndef GAME_HPP
#define GAME_HPP
/// <summary>
/// include guards used so we don't process this file twice
/// same as #pragma once
/// Don't forget the endif at the bottom
/// </summary>
#include <SFML/Graphics.hpp>
#include "Globals.h"
#include "Player.h"
#include "Enemy.h"
#include "Menu.h"
#include "Pickup.h"
#include "CollisionManager.h"
#include "Grid.h"
#include "World.h"
#include "Environment.h"


class Game
{
public:
	Game();
	~Game();
	/// <summary>
	/// main method for game
	/// </summary>
	void run();

private:

	void processEvents();
	void processKeys(sf::Event t_event);
	void update(sf::Time t_deltaTime);
	void render();


	sf::RenderWindow m_window; // main SFML window
    World m_world;

	bool m_exitGame; // control exiting game

	Menu m_gameMenu;
};

#endif // !GAME_HPP

