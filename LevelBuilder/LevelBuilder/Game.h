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

#include <iostream>
#include <algorithm>
#include <fstream>

#include <SFML/Graphics.hpp>

#include "json.hpp"
#include "Globals.h"
#include "Button.h"
#include "Tile.h"
#include "Graph.h"

typedef GraphArc<NodeData, float> Arc;
typedef GraphNode<NodeData, float> Node;

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
	void setupOptions();
	void manageClicks(sf::Event t_event);
	void assignText();

	void calculateUcsWaypoints();

	sf::RenderWindow m_window; // main SFML window
	bool m_exitGame; // control exiting game

	sf::Sprite mapArea;
	sf::Texture mapTexture;

	Button* saveButton;
	Button* deleteButton;
	Button* rotateButton;
	Button* upButton;
	Button* downButton;
	Button* leftButton;
	Button* rightButton;
	sf::Text currentCategoryText;
	int currentCategory = 0;
	std::string titles[NUM_CATEGORIES];
	bool passable[NUM_CATEGORIES];
	std::vector<Button*> tileOptions[6];
	sf::RectangleShape topScrollBlock;
	sf::RectangleShape bottomScrollBlock;
	Button* selectedButton;
	sf::Texture m_texture;
	sf::Font m_font;

	static const int mapSize = (screen_Height / tileSize) * (screen_Width / tileSize);

	Graph<NodeData, float> m_graph;
	std::map<std::string, std::vector<Node*>> m_ucsPaths;
	std::vector<int> m_waypointsIdx;
	std::vector<sf::RectangleShape> m_ucsDebugTiles;

	Tile* m_MapTiles[mapSize];
	bool isDeleting = false;
	bool isRotating = false;

	sf::View view;
	sf::Vector2f oldClickPos;
	bool isRightBtnPressed = false;
};

#endif // !GAME_HPP

