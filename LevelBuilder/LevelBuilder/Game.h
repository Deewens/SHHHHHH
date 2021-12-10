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
#include "Button.h"
#include <iostream>
#include "Tile.h"

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
	
	void setupSprite();
	void setupHUD();
	void manageClicks(sf::Event t_event);

	sf::RenderWindow m_window; // main SFML window
	bool m_exitGame; // control exiting game

	sf::Sprite mapArea;
	sf::Texture mapTexture;

	static const int cols = screen_Width / tileSize;
	sf::Vertex m_colLine[cols * 2];
	static const int rows = screen_Height / tileSize;
	sf::Vertex m_rowLine[rows * 2];

	Button* saveButton;
	Button* deleteButton;
	Button* upButton;
	Button* downButton;
	std::vector<Button*> tileOptions;
	Button* selectedButton;
	sf::Texture m_texture;

	static const int mapSize = (screen_Height / tileSize) * (screen_Width / tileSize);
	Tile* m_MapTiles[mapSize];

	bool isDeleting = false;
};

#endif // !GAME_HPP

