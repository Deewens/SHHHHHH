#include "World.h"

World::World(sf::RenderWindow &window) :
    m_window(window),
    m_worldView(window.getDefaultView()),
    m_worldBounds(0.f, 0.f, m_worldView.getSize().x / 2.f,m_worldBounds.height - m_worldView.getSize().y / 2.f),
    m_spawnPosition(100.f, 100.f)
{
    loadTextures();
    buildScenes();

    m_worldView.setCenter(m_spawnPosition);
}

void World::update(sf::Time dt)
{
    //m_worldView.move(0.f, -50.f * dt.asSeconds());
    m_player.update(dt.asSeconds());
    m_enemy.update(dt.asSeconds());
    m_grid.update();
    checkCollisions();
    collisions.update();

    m_worldView.setCenter(m_player.getPosition());
    
    m_gameMenu.update((sf::Vector2f)sf::Mouse::getPosition(m_window));
}

void World::draw()
{
    m_window.setView(m_worldView);
    m_window.draw(m_gameMenu);
    m_environment.render(m_window);
    m_window.draw(m_player);
    m_window.draw(m_pickup);
    m_window.draw(m_enemy);
    m_enemy.renderVisionCone(m_window);
    m_window.draw(m_grid);
    collisions.renderNoises(m_window);
}

/// <summary>
/// Load all the needed textures for the game
/// </summary>
void World::loadTextures()
{

}

void World::buildScenes()
{
    m_grid = Grid(screen_Height / tileSize, screen_Width / tileSize);
}

void World::processEvents(sf::Event event)
{
    m_player.processEvents(event);
}

void World::checkCollisions()
{
	collisions.check(m_player, m_enemy);
	collisions.check(m_player, m_pickup);
    m_enemy.visionConeCollisionCheck(m_player.getPosition());
    collisions.check(m_player, m_environment);
}
