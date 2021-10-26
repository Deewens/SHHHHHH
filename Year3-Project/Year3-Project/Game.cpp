/// <summary>
/// @author Peter Lowe
/// @date May 2019
///
/// you need to change the above lines or lose marks
/// </summary>

#include "Game.h"
#include <iostream>



/// <summary>
/// default constructor
/// setup the window properties
/// load and setup the text 
/// load and setup thne image
/// </summary>
Game::Game() :
	m_window{ sf::VideoMode{ screen_Width, screem_Height, 32U }, "SHHHH...!" },
	m_exitGame{false} //when true game will exit
{
	m_gameMenu.Init();
	m_grid = Grid(screem_Height / tileSize, screen_Width / tileSize);

}

/// <summary>
/// default destructor we didn't dynamically allocate anything
/// so we don't need to free it, but mthod needs to be here
/// </summary>
Game::~Game()
{
}


/// <summary>
/// main game loop
/// update 60 times per second,
/// process update as often as possible and at least 60 times per second
/// draw as often as possible but only updates are on time
/// if updates run slow then don't render frames
/// </summary>
void Game::run()
{	
	sf::Clock clock;
	sf::Time timeSinceLastUpdate = sf::Time::Zero;
	const float fps{ 60.0f };
	sf::Time timePerFrame = sf::seconds(1.0f / fps); // 60 fps
	while (m_window.isOpen())
	{
		processEvents(); // as many as possible
		timeSinceLastUpdate += clock.restart();
		while (timeSinceLastUpdate > timePerFrame)
		{
			timeSinceLastUpdate -= timePerFrame;
			processEvents(); // at least 60 fps
			update(timePerFrame); //60 fps
		}
		render(); // as many as possible
	}
}
/// <summary>
/// handle user and system events/ input
/// get key presses/ mouse moves etc. from OS
/// and user :: Don't do game update here
/// </summary>
void Game::processEvents()
{
	sf::Event newEvent;
	while (m_window.pollEvent(newEvent))
	{
		if ( sf::Event::Closed == newEvent.type) // window message
		{
			m_exitGame = true;
		}
		if (sf::Event::KeyPressed == newEvent.type) //user pressed a key
		{
			processKeys(newEvent);
		}
	}
}


/// <summary>
/// deal with key presses from the user
/// </summary>
/// <param name="t_event">key press event</param>
void Game::processKeys(sf::Event t_event)
{
	if (sf::Keyboard::Escape == t_event.key.code)
	{
		m_exitGame = true;
	}
}

/// <summary>
/// Update the game world
/// </summary>
/// <param name="t_deltaTime">time interval per frame</param>
void Game::update(sf::Time t_deltaTime)
{

	if (m_exitGame)
	{
		m_window.close();
	}

	m_player.update();
    m_enemy.update();

	switch (m_gameState)
	{
	case GameState::MENU:
		break;
	case GameState::GAMEPLAY:
		m_player.update();
		checkCollisions();
		collisions.update();
		m_grid.update();
		break;
	case GameState::EXIT:
		break;
	case GameState::OPTIONS:
		break;
	default:
		break;
	}
	m_gameMenu.update((sf::Vector2f)sf::Mouse::getPosition(m_window));

}

/// <summary>
/// draw the frame and then switch buffers
/// </summary>
void Game::render()
{
	m_window.clear(sf::Color::Black);

	switch (m_gameState)
	{
	case GameState::MENU:
		m_gameMenu.render(m_window);
		break;
	case GameState::GAMEPLAY:
		m_pickup.render(m_window);
		m_player.render(m_window);
		m_gameMenu.render(m_window);
        m_enemy.render(m_window);
		collisions.renderNoises(m_window);
		m_grid.render(m_window);
        break;
	case GameState::EXIT:
		break;
	case GameState::OPTIONS:
		m_gameMenu.render(m_window);
		break;
	default:
		break;
	}
	//m_gameMenu.draw(m_window);
	m_window.display();
}

void Game::checkCollisions()
{
	collisions.check(m_player, m_enemy);
	collisions.check(m_player, m_pickup);
}

