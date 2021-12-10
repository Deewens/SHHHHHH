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
	m_window{ sf::VideoMode{ screen_Width + menu_Width, screen_Height, 32U }, "LevelLoader" },
	m_exitGame{false} //when true game will exit
{
	setupSprite(); // load texture
	setupHUD();
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
	manageClicks(newEvent);
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
}

/// <summary>
/// draw the frame and then switch buffers
/// </summary>
void Game::render()
{
	m_window.clear(sf::Color::White);
	m_window.draw(mapArea);
	saveButton->render(m_window);
	deleteButton->render(m_window);
	upButton->render(m_window);
	downButton->render(m_window);
	for (Button* button : tileOptions)
	{
		button->render(m_window);
	}
	for (int i = 0; i < mapSize; i++)
	{
		if (m_MapTiles[i] != nullptr)
		{
			m_MapTiles[i]->render(m_window);
		}
	}
	m_window.draw(m_colLine, cols * 2, sf::Lines);
	m_window.draw(m_rowLine, rows * 2, sf::Lines);
	m_window.display();
}

/// <summary>
/// load the texture and setup the sprite for the logo
/// </summary>
void Game::setupSprite()
{
	if (!mapTexture.loadFromFile("ASSETS/IMAGES/Map.png"))
	{

	}
	mapArea.setTexture(mapTexture);
	mapArea.setScale(sf::Vector2f((float)screen_Width / (float)mapTexture.getSize().x, (float)screen_Height / (float)mapTexture.getSize().y));
	
	for (int i = 0; i < cols; i++)
	{
		m_colLine[i * 2] = sf::Vertex(sf::Vector2f{ (float)(i * tileSize), 0 });
		m_colLine[i * 2 + 1] = sf::Vertex(sf::Vector2f{ (float)(i * tileSize), screen_Height });
		m_colLine[i * 2].color = sf::Color::Black;
		m_colLine[i * 2 + 1].color = sf::Color::Black;
	}

	for (int i = 0; i < rows; i++)
	{
		m_rowLine[i * 2] = sf::Vertex(sf::Vector2f{ 0,(float)(i * tileSize) });
		m_rowLine[i * 2 + 1] = sf::Vertex(sf::Vector2f{ screen_Width ,(float)(i * tileSize) });
		m_rowLine[i * 2].color = sf::Color::Black;
		m_rowLine[i * 2 + 1].color = sf::Color::Black;
	}
}

void Game::setupHUD()
{
	saveButton = new Button(sf::Vector2f(screen_Width, screen_Height - 100), sf::Vector2f(menu_Width, 100), sf::Color::Green, "Save", 40, sf::Color::Black);
	deleteButton = new Button(sf::Vector2f(screen_Width + menu_Width / 4, screen_Height - 160), sf::Vector2f(menu_Width / 2, 50), sf::Color::Red, 
		"Delete", 30, sf::Color::Black);
	upButton = new Button(sf::Vector2f(screen_Width + (3 * menu_Width / 8), 20), sf::Vector2f(menu_Width / 4, 50), sf::Color::Cyan,
		"^", 30, sf::Color::Black);
	downButton = new Button(sf::Vector2f(screen_Width + (3 * menu_Width / 8), screen_Height - 230), sf::Vector2f(menu_Width / 4, 50), sf::Color::Cyan,
		"v", 30, sf::Color::Black);

	Button* temp;
	int imageCount = 8;
	std::string images[8] = { "SFML-LOGO.png",
		"SFML-LOGO.png",
		"SFML-LOGO.png",
		"SFML-LOGO.png",
		"SFML-LOGO.png",
		"SFML-LOGO.png",
		"SFML-LOGO.png",
		"SFML-LOGO.png" };
	for (int i = 0; i < imageCount; i++)
	{
		if (!m_texture.loadFromFile("ASSETS/IMAGES/" + images[i]))
		{

		}
		temp = new Button(sf::Vector2f(screen_Width + (menu_Width / 2 - 75), 100 + (200 * i)), sf::Vector2f(150, 150), m_texture, images[i]);
		tileOptions.push_back(temp);
	}
}

void Game::manageClicks(sf::Event t_event)
{
	sf::Vector2i click = sf::Mouse::getPosition(m_window);
	//holding down mouse
	if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
	{
		if (upButton->isInside(click))
		{
			for (Button* button : tileOptions)
			{
				button->moveUp(0.2f);
			}
		}
		if (downButton->isInside(click))
		{
			for (Button* button : tileOptions)
			{
				button->moveDown(0.2f);
			}
		}
	}
	if (t_event.type == sf::Event::MouseButtonReleased && t_event.mouseButton.button == sf::Mouse::Left)
	{
		//check if you interacted with the ui
		if (click.x >= screen_Width && click.x < screen_Width + menu_Width &&
			click.y >= 0 && click.y <= screen_Height)
		{
			for (Button* button : tileOptions)
			{
				if (button->isInside(click))
				{
					if (selectedButton != nullptr)
						selectedButton->setSelected(false);
					selectedButton = button;
					button->setSelected(true);
					if (isDeleting)
					{
						isDeleting = false;
						deleteButton->setSelected(false);
					}
				}
			}
			if (deleteButton->isInside(click))
			{
				isDeleting = true;
				deleteButton->setSelected(true);
				if (selectedButton != nullptr)
				{
					selectedButton->setSelected(false);
					selectedButton = nullptr;
				}
			}
		}
		//check if you clicked on the map
		else if (click.x <= screen_Width && click.x > 0 &&
			click.y >= 0 && click.y <= screen_Height)
		{
			int tileNum = trunc(click.x / tileSize) + (trunc(click.y / tileSize) * (screen_Width / tileSize));
			if (isDeleting)
			{
				m_MapTiles[tileNum] = nullptr;
			}
			else if (selectedButton != nullptr)
			{
				m_MapTiles[tileNum] = new Tile(selectedButton, tileNum);
			}
		}
	}
}
