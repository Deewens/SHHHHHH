/// <summary>
///
/// </summary>

#include <algorithm>

#include "Game.h"

/// <summary>
/// default constructor
/// setup the window properties
/// load and setup the text 
/// load and setup thne image
/// </summary>
Game::Game() :
	m_window(sf::VideoMode{ screen_Width, screen_Height, 32U }, "SHHHH...!"),
	m_exitGame(false), //when true game will exit
    m_spawnPosition(100.f, 100.f){

    loadSounds();
    m_player.loadSoundHolder(m_sounds);
    m_enemy.loadSoundHolder(m_sounds);

    m_gameMenu.Init();
    m_grid = Grid(screen_Height / tileSize, screen_Width / tileSize);
    pauseMenuSetUp();
    setupEnvironment();
    setUpPickUps();
    m_worldView.reset(sf::FloatRect(m_player.getPosition().x, m_player.getPosition().y, screen_Width / 2, screen_Height / 2));
    m_menuView.reset(sf::FloatRect(0,0, screen_Width, screen_Height));
    m_grid.debug();
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
        if (GameState::GAMEPLAY == m_gameState)
        {
            m_player.processEvents(newEvent);
            if (sf::Event::KeyReleased == newEvent.type)
            {
                if (newEvent.key.code == sf::Keyboard::D)
                {
                    m_grid.toggleDraw();
                }
            }
        }
        else if (GameState::PAUSE == m_gameState)
        {
            if (sf::Event::KeyReleased == newEvent.type)
            {
                if (newEvent.key.code == sf::Keyboard::P)
                {
                    m_gameState = GameState::GAMEPLAY;
                }
            }
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
    sf::Vector2f playerPos;
    sf::Vector2f enemyPos;
    float dist;
    float volume;
	switch (m_gameState)
	{
        case GameState::MENU:
			m_gameMenu.update((sf::Vector2f)sf::Mouse::getPosition(m_window));
            break;
        case GameState::GAMEPLAY:
            m_window.setView(m_worldView);
            m_hud.update(m_worldView.getCenter());
            m_player.update(t_deltaTime);
            m_enemy.update(t_deltaTime);
            checkCollisions();
            checkPickUps();
            collisions.update();
            cellIdFinder(m_player.getPosition());
            cameraMovement(t_deltaTime); 
            if (m_player.m_throw[0])
            {
                m_pickup[0]->throwPickUp(m_player.getRotation(), m_player.getPosition(), m_player.m_throwPower());
                m_player.m_throw[0] = false;
            }
            if (m_player.m_throw[1])
            {
                m_pickup[0]->throwPickUp(m_player.getRotation(), m_player.getPosition(), m_player.m_throwPower());
                m_player.m_throw[1] = false;
            }
            for (int i = 0; i < 2; i++)
            {
                m_pickup[i]->move();
            }
            cameraMovement(t_deltaTime);

            dist = m_player.getDistance(m_enemy);
            volume = std::max<float>(0.f, 100.f - 100.f/300.f * dist);
            m_enemy.changeSoundsVolume(volume);
            break;
        case GameState::EXIT:
            m_exitGame = true;
            break;
        case GameState::OPTIONS:
            std::cout << "option" << std::endl;
            break;
        case GameState::PAUSE:
            m_pauseRect.setPosition(m_worldView.getCenter());
            std::cout << "pause" << std::endl;
            break;
        default:
            break;
	}
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

            m_window.clear(sf::Color(0, 157, 196));
            m_window.setView(m_worldView);

            for (Environment env : m_environment)
            {
                env.render(m_window);
            }
            for (int i=0 ; i<2 ; i++)
            {
                if (m_pickupCollected[i] == false)
                {
                    m_window.draw(*m_pickup[i]);
                }
            }
            m_window.draw(m_player);
            m_window.draw(m_enemy);
            m_enemy.renderVisionCone(m_window);
            m_window.draw(m_grid);
            collisions.renderNoises(m_window);
            m_player.renderPowerBar(m_window);
            m_hud.render(m_window);
            break;
        case GameState::EXIT:
            break;
        case GameState::OPTIONS:
            m_window.draw(m_gameMenu);
            break;
        case GameState::PAUSE:
            m_window.setView(m_worldView);
            for (Environment env : m_environment)
            {
                env.render(m_window);
            }
            m_window.draw(m_player);
            //m_window.draw(m_pickup);
            m_window.draw(m_enemy);
            m_enemy.renderVisionCone(m_window);
            m_window.draw(m_grid);
            collisions.renderNoises(m_window);
            m_window.draw(m_pauseRect);
            break;
        default:
            break;
	}
	m_window.display();
}

void Game::checkCollisions()
{
    collisions.check(m_player, m_enemy);

    if (collisions.check(m_player, *m_pickup[0]))
    {
        m_pickupCollected[0] = true;
        m_hud.m_pickUpHud[0] = true;
        m_player.m_readyToTHrow[0] = true;
    }
    if (collisions.check(m_player, *m_pickup[1]))
    {
        m_pickupCollected[1] = true;
        m_hud.m_pickUpHud[1] = true;
        m_player.m_readyToTHrow[1] = true;
    }   
    
    for (Environment env : m_environment)
    {
        if (env.isImpassable())
            collisions.check(m_player, env);
    }
}

void Game::pauseMenuSetUp()
{
    m_pauseRect.setSize(sf::Vector2f(screen_Width/4, screen_Height/4));
    m_pauseRect.setFillColor(sf::Color(225, 0, 0, 100));
    m_pauseRect.setOrigin(screen_Width/8,screen_Height/8);
}

void Game::setupEnvironment()
{
    if (!m_groundTexture.loadFromFile("ASSETS/IMAGES/sprite_sheets/ground_sprite_sheet.png"))
        std::cout << "problem loading character texture" << std::endl;

    std::ifstream spriteSheetData("scene.json");
    nlohmann::json json;
    spriteSheetData >> json;

    nlohmann::json scene = json["scene"];
    // Build scene from the json file
    for (auto& el : scene)
    {
        m_environment.push_back(Environment(m_groundTexture, el["spriteName"], el["gridIndex"], screen_Height / tileSize, screen_Width / tileSize, el["rotation"]));
    }
    // Add impassable wall
    //sf::RectangleShape wall;
    //wall.setFillColor(sf::Color::Red);
    //m_environment.push_back(Environment(wall, 73, screen_Height / tileSize, screen_Width / tileSize, 0, true));
}

void Game::cameraMovement(sf::Time dt)
{
    sf::Vector2f movement = m_player.getPosition() - m_worldView.getCenter();
    m_worldView.move(movement * dt.asSeconds());
}

int Game::cellIdFinder(sf::Vector2f t_targetLocation)
{
    int m_id = floor(t_targetLocation.x / tileSize) + (floor(t_targetLocation.y / tileSize) * m_gridCols);

    //std::cout << std::to_string(m_id) << std::endl;

    return m_id;
}
void Game::setUpPickUps()
{
    m_pickup[0] =new Pickup(22);
    m_pickup[1] =new Pickup(43);
}
void Game::checkPickUps()
{
    if (m_player.m_readyToTHrow[0] == false)
    {
        m_hud.m_pickUpHud[0] = false;
    }
    if (m_player.m_readyToTHrow[1] == false)
    {
        m_hud.m_pickUpHud[1] = false;
    }
}

void Game::loadSounds()
{
    std::string path = "ASSETS/SOUNDS";
    m_sounds.load(Sounds::Footsteps_Walk_Sand1, path + "/Footsteps/Sand/Walk/Footsteps_Walk_Sand1M.wav");
    m_sounds.load(Sounds::Footsteps_Walk_Sand2, path + "/Footsteps/Sand/Walk/Footsteps_Walk_Sand2M.wav");
    m_sounds.load(Sounds::Footsteps_Walk_Sand3, path + "/Footsteps/Sand/Walk/Footsteps_Walk_Sand3M.wav");
    m_sounds.load(Sounds::Footsteps_Walk_Sand4, path + "/Footsteps/Sand/Walk/Footsteps_Walk_Sand4M.wav");
    m_sounds.load(Sounds::Footsteps_Walk_Sand5, path + "/Footsteps/Sand/Walk/Footsteps_Walk_Sand5M.wav");
    m_sounds.load(Sounds::Footsteps_Walk_Sand6, path + "/Footsteps/Sand/Walk/Footsteps_Walk_Sand6M.wav");
    m_sounds.load(Sounds::Footsteps_Walk_Sand7, path + "/Footsteps/Sand/Walk/Footsteps_Walk_Sand7M.wav");
    m_sounds.load(Sounds::Footsteps_Walk_Sand8, path + "/Footsteps/Sand/Walk/Footsteps_Walk_Sand8M.wav");

    m_sounds.load(Sounds::Footsteps_Run_Sand1, path + "/Footsteps/Sand/Run/Footsteps_Run_Sand1M.wav");
    m_sounds.load(Sounds::Footsteps_Run_Sand2, path + "/Footsteps/Sand/Run/Footsteps_Run_Sand2M.wav");
    m_sounds.load(Sounds::Footsteps_Run_Sand3, path + "/Footsteps/Sand/Run/Footsteps_Run_Sand3M.wav");
    m_sounds.load(Sounds::Footsteps_Run_Sand4, path + "/Footsteps/Sand/Run/Footsteps_Run_Sand4M.wav");
    m_sounds.load(Sounds::Footsteps_Run_Sand5, path + "/Footsteps/Sand/Run/Footsteps_Run_Sand5M.wav");
    m_sounds.load(Sounds::Footsteps_Run_Sand6, path + "/Footsteps/Sand/Run/Footsteps_Run_Sand6M.wav");
    m_sounds.load(Sounds::Footsteps_Run_Sand7, path + "/Footsteps/Sand/Run/Footsteps_Run_Sand7M.wav");
    m_sounds.load(Sounds::Footsteps_Run_Sand8, path + "/Footsteps/Sand/Run/Footsteps_Run_Sand8M.wav");

    m_sounds.load(Sounds::Footsteps_Sneak_Sand1, path + "/Footsteps/Sand/Sneak/Footsteps_Sneak_Sand1M.wav");
    m_sounds.load(Sounds::Footsteps_Sneak_Sand2, path + "/Footsteps/Sand/Sneak/Footsteps_Sneak_Sand2M.wav");
    m_sounds.load(Sounds::Footsteps_Sneak_Sand3, path + "/Footsteps/Sand/Sneak/Footsteps_Sneak_Sand3M.wav");
    m_sounds.load(Sounds::Footsteps_Sneak_Sand4, path + "/Footsteps/Sand/Sneak/Footsteps_Sneak_Sand4M.wav");
}




