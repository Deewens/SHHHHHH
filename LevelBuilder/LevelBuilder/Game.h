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

const int screen_Width = 1800;
const int menu_Width = 300;
const int screen_Height = 1350;

static const int tileSize{ 90 };

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

	sf::RenderWindow m_window; // main SFML window
	bool m_exitGame; // control exiting game

	sf::Sprite mapArea;
	sf::Texture mapTexture;

	static const int cols = screen_Width / tileSize;
	sf::Vertex m_colLine[cols * 2];
	static const int rows = screen_Height / tileSize;
	sf::Vertex m_rowLine[rows * 2];
	

};

#endif // !GAME_HPP

