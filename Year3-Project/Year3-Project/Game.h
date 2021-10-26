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
#include "HUD.h"


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
	void checkCollisions();
	

	sf::RenderWindow m_window; // main SFML window
	bool m_exitGame; // control exiting game
	CollisionManager collisions;

	Player m_player;

    Enemy m_enemy;

	Pickup m_pickup;

	Menu m_gameMenu;

	Grid m_grid;

	HUD m_hud;

};

#endif // !GAME_HPP

