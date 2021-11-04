/// <summary>
/// @author Peter Lowe
/// @date May 2019
///
/// you need to change the above lines or lose marks
/// </summary>

#include "Game.h"

/// <summary>
/// default constructor
/// setup the window properties
/// load and setup the text 
/// load and setup thne image
/// </summary>
Game::Game() :
	m_window(sf::VideoMode{ screen_Width, screem_Height, 32U }, "SHHHH...!"),
	m_exitGame(false), //when true game will exit
    m_world(m_window)
{
    m_gameMenu.Init();
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
        m_world.processEvents(newEvent);
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

	switch (m_gameState)
	{
        case GameState::MENU:
            break;
        case GameState::GAMEPLAY:
            m_world.update(t_deltaTime);
            break;
        case GameState::EXIT:
            m_exitGame = true;
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
            m_window.draw(m_gameMenu);
            break;
        case GameState::GAMEPLAY:
            //m_window.draw(m_gameMenu);
            m_world.draw();
            //m_window.setView(m_window.getDefaultView());
            break;
        case GameState::EXIT:
            break;
        case GameState::OPTIONS:
            m_window.draw(m_gameMenu);
            break;
        default:
            break;
	}
	//m_gameMenu.draw(m_window);
	m_window.display();
}

