#ifndef YEAR3_PROJECT_WORLD_H
#define YEAR3_PROJECT_WORLD_H

#include <SFML/Graphics.hpp>
#include "Player.h"
#include "Player.h"
#include "Enemy.h"
#include "Menu.h"
#include "Pickup.h"
#include "Grid.h"
#include "CollisionManager.h"


/// <summary>
/// This class will take care of the rendering. It will contains all the data related to rendering.
/// </summary>
class World : private sf::NonCopyable
{
private:
    sf::RenderWindow& m_window; // Window where the world will be rendered
    sf::View m_worldView; // Take care of the view (camera)

    sf::FloatRect m_worldBounds;
    sf::Vector2f m_spawnPosition;

    CollisionManager collisions;

    // Game entities
    Player m_player;
    Enemy m_enemy;
    Pickup m_pickup;
    Grid m_grid;
    Environment m_environment;
    Menu m_gameMenu;


public:
    explicit World(sf::RenderWindow& window);
    void update(sf::Time dt);
    void draw(); // Draw element to the view

    void checkCollisions();

    // Temporary methods
    void processEvents(sf::Event event);

private:
    void loadTextures(); // Will be used to load the different textures of the game (later)
    void buildScenes(); // Will be used to initialize the scene of the game (add the different entities-game objects with their correct position, texture, etc.)
};


#endif //YEAR3_PROJECT_WORLD_H
