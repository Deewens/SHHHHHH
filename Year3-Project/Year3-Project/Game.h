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
#include <fstream>
#include <SFML/Graphics.hpp>
#include <algorithm>
#include "Globals.h"
#include "Player.h"
#include "Enemy.h"
#include "Menu.h"
#include "Pickup.h"
#include "CollisionManager.h"
#include "Graph.h"
#include "Environment.h"
#include "HUD.h"
#include "SoundHolder.h"
#include "Utils.h"
#include "NodeData.h"
#include "Goal.h"

using json = nlohmann::json;

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
    void checkCollisions();

    void setupBase();
    void setupEnvironment();
    void loadSounds();

    void cameraMovement(sf::Time dt);

    int cellIdFinder(sf::Vector2f t_targetLocation);

    void setUpSpecial(json& levelData);

    void checkPickUps();

    void loadNewLevel();

    

    sf::RenderWindow m_window; // main SFML window

	bool m_exitGame; // control exiting game

    sf::View m_worldView; // Take care of the view (camera)


    sf::FloatRect m_worldBounds;
    sf::Vector2f m_spawnPosition;

    CollisionManager collisions;

    // Game entities
    Player m_player;
    std::vector<Enemy*> m_zombies;
    std::vector<Pickup*> m_pickups;
    Goal* m_goal;

    Graph<NodeData, float> m_grid;
    std::vector<Environment> m_ground; // Everything related to the ground of the scene
    std::vector<Environment> m_environment; // Environment/tiles of the level (wall, tree, water...)
    Menu m_gameMenu;
    HUD m_hud;

    sf::Texture m_spriteSheet;

    sf::Font m_font;

    sf::Text m_playerCoordsDebugText;

    SoundHolder m_sounds;

    int noiseCounter = 0;
};

#endif // !GAME_HPP

