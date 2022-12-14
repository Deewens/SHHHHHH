/// <summary>
///
/// </summary>

#include "Game.h"

void visit(Node *t_node)
{
    std::cout << "Visiting: " << t_node->m_data.id << std::endl;
}

/// <summary>
/// default constructor
/// setup the window properties
/// load and setup the text 
/// load and setup thne image
/// </summary>
Game::Game() :
        m_window(sf::VideoMode{(unsigned int) screen_Width, (unsigned int) screen_Height, 32U}, "SHHHH...!"),
        m_exitGame(false), //when true game will exit
        m_grid(Graph<NodeData, float>(screen_Height / tileSize, screen_Width / tileSize, 300)),
        m_player(m_spriteSheet)
{

    if (!m_font.loadFromFile("ASSETS\\FONTS\\ariblk.ttf"))
    {
        std::cout << "Problem loading font" << std::endl;
    }

    m_playerCoordsDebugText.setFont(m_font);
    m_playerCoordsDebugText.setCharacterSize(20);
    m_playerCoordsDebugText.setFillColor(sf::Color::White);
    m_playerCoordsDebugText.setPosition(0, 0);

    m_loadingText.setFont(m_font);
    m_loadingText.setCharacterSize(100);
    m_loadingText.setFillColor(sf::Color::White);
    m_loadingText.setString("Loading....");
    m_loadingText.setOrigin(m_loadingText.getGlobalBounds().width / 2, m_loadingText.getGlobalBounds().height / 2);
    m_loadingText.setPosition(screen_Width / 2, screen_Height / 2);


    m_window.clear(sf::Color::Black);
    m_window.draw(m_loadingText);
    m_window.display();

    openMusicFiles();
    loadSounds();
    m_player.loadSoundHolder(m_sounds);

    m_gameMenu.Init();

    setupBase();
    m_grid.debug(m_font);

    // Fill the graph with all the arcs using the neighbours algorithm
    for (int i = 0; i < m_grid.getNodes().size(); i++)
    {
        std::map<int, float> neighbours = m_grid.getNeighbours(i);
        for (auto neighbour: neighbours)
        {
            m_grid.addArc(i, neighbour.first, neighbour.second);
        }
    }
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
    const float fps{60.0f};
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
        if (sf::Event::Closed == newEvent.type) // window message
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
                    for (Enemy *m_enemy: m_zombies)
                    {
                        m_enemy->changeDebug();
                    }
                }
            }
        } else if (GameState::PAUSE == m_gameState)
        {
            if (sf::Event::KeyReleased == newEvent.type)
            {
                if (newEvent.key.code == sf::Keyboard::P)
                {
                    m_gameState = GameState::GAMEPLAY;
                }
                if (newEvent.key.code == sf::Keyboard::E)
                {
                    m_gameState = GameState::MENU;
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

    if (sf::Keyboard::F1 == t_event.key.code)
    {
        m_playerCoordsDebugText.setString(
                "X: " + std::to_string(m_player.getPosition().x) + " - Y: " + std::to_string(m_player.getPosition().y));
    }

    if (sf::Keyboard::F2 == t_event.key.code)
    {
/*        int enemy = m_grid.getClosestWaypoint(m_enemy.getPosition());
        int player = Utils::vectorToNode(m_player.getPosition());

        std::cout << "Player node: " << player << std::endl;
        std::cout << "Enemy node: " << enemy << std::endl;

/*        std::vector<Node*> path;
        m_grid.clearMarks();
        m_grid.aStar(m_grid.nodeIndex(player), m_grid.nodeIndex(waypoint), path);*/

/*        for (auto& node : path)
            std::cout << node->m_data.id << " ";

        std::cout << std::endl;*/
/*        for (Enemy* m_enemy : m_zombies)
        {
            m_enemy->moveTo(m_player.getPosition());
        }*/
    }
}

/// <summary>
/// Update the game world
/// </summary>
/// <param name="t_deltaTime">time interval per frame</param>
void Game::update(sf::Time t_deltaTime)
{
    if (m_exitGame)
        m_window.close();

    float dist;
    float volume;
    switch (m_gameState)
    {
        case GameState::MENU:
            m_window.setView(m_worldView);
            if (m_gameMenu.update((sf::Vector2f) sf::Mouse::getPosition(m_window), m_window))
            {
                level = 0;
                loadNewLevel();
            }

            if (m_inGameMusic.getStatus() == sf::SoundSource::Playing)
                m_inGameMusic.stop();

            if (m_menuMusic.getStatus() == sf::SoundSource::Stopped)
                m_menuMusic.play();

            break;
        case GameState::GAMEPLAY:
        {
            if (m_menuMusic.getStatus() == sf::SoundSource::Playing)
                m_menuMusic.stop();

            if (m_inGameMusic.getStatus() == sf::SoundSource::Stopped)
                m_inGameMusic.play();

            m_window.setView(m_worldView);
            m_hud.update(m_worldView.getCenter());
            m_player.update(t_deltaTime, m_worldView.getCenter());
            for (Enemy *m_enemy: m_zombies)
            {
                m_enemy->update(t_deltaTime);
                dist = m_player.getDistance(*m_enemy);
                // Change the volume of the sounds depending on the distance with the player
                volume = std::max<float>(0.f, 100.f - 100.f / 300.f * dist);
                m_enemy->changeSoundsVolume(volume);
            }
            checkCollisions();
            checkPickUps();
            collisions.update();
            cellIdFinder(m_player.getPosition());
            cameraMovement(t_deltaTime);
        }
            break;
        case GameState::EXIT:
            m_exitGame = true;
            break;
        case GameState::HELP:
            m_gameMenu.update((sf::Vector2f) sf::Mouse::getPosition(m_window), m_window);
            break;
        case GameState::PAUSE:
            m_window.setView(m_worldView);
            m_gameMenu.m_pauseRect.setPosition(m_worldView.getCenter());
            m_gameMenu.GameBackButton1.Init(
                    sf::Vector2f(m_worldView.getCenter().x - 150, m_worldView.getCenter().y - 100), m_font,
                    sf::Vector2f(310, 40), sf::Vector2f(0, 0));
            m_gameMenu.GameBackButton2.Init(sf::Vector2f(m_worldView.getCenter().x - 170, m_worldView.getCenter().y),
                                            m_font, sf::Vector2f(350, 40), sf::Vector2f(0, 0));

            //m_gameMenu.update((sf::Vector2f)sf::Mouse::getPosition(m_window));
            break;
        case GameState::WINLEVEL:
            m_window.setView(m_worldView);
            m_gameMenu.update((sf::Vector2f) sf::Mouse::getPosition(m_window), m_window);
            break;
        case GameState::LOSE:
            m_window.setView(m_worldView);
            m_gameMenu.update((sf::Vector2f) sf::Mouse::getPosition(m_window), m_window);
            break;
        case GameState::WINGAME:
            m_window.setView(m_worldView);
            m_gameMenu.update((sf::Vector2f) sf::Mouse::getPosition(m_window), m_window);
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
            m_worldView.reset(sf::FloatRect(0, 0, screen_Width, screen_Height));
            m_window.setView(m_worldView);
            m_window.draw(m_gameMenu);
            break;
        case GameState::GAMEPLAY:
            m_window.clear(sf::Color(0, 157, 196));
            m_worldView.reset(sf::FloatRect(m_player.getPosition().x - screen_Width / 4,
                                            m_player.getPosition().y - screen_Height / 4, screen_Width / 2,
                                            screen_Height / 2));
            m_window.setView(m_worldView);

            for (Environment env: m_ground)
            {
                env.render(m_window);
            }

            for (auto &object: m_environment)
            {
                object.render(m_window);
            }

            for (Pickup *bottle: m_pickups)
            {
                m_window.draw(*bottle);
            }

            m_window.draw(m_player);
            for (Enemy *m_enemy: m_zombies)
            {
                m_window.draw(*m_enemy);
                m_enemy->renderVisionCone(m_window);
            }
            m_goal->render(m_window);
            m_window.draw(m_grid);
            collisions.renderNoises(m_window);
            m_hud.render(m_window);

            m_window.setView(m_window.getDefaultView());
            m_window.draw(m_playerCoordsDebugText);
            m_player.renderPowerBar(m_window);

            break;
        case GameState::EXIT:
            break;
        case GameState::HELP:
            m_window.draw(m_gameMenu);
            break;
        case GameState::PAUSE:
            m_window.setView(m_worldView);
            for (Environment env: m_ground)
            {
                env.render(m_window);
            }

            for (auto &object: m_environment)
            {
                object.render(m_window);
            }

            m_window.draw(m_player);
            //m_window.draw(m_pickup);
            for (Enemy *m_enemy: m_zombies)
            {
                m_window.draw(*m_enemy);
                m_enemy->renderVisionCone(m_window);
            }
            m_goal->render(m_window);
            m_window.draw(m_grid);
            collisions.renderNoises(m_window);
            m_window.draw(m_gameMenu);
            break;
        case GameState::WINLEVEL:
            m_worldView.reset(sf::FloatRect(0, 0, screen_Width, screen_Height));
            m_window.draw(m_gameMenu);
            break;
        case GameState::LOSE:
            m_worldView.reset(sf::FloatRect(0, 0, screen_Width, screen_Height));
            m_window.draw(m_gameMenu);
            break;
        case GameState::WINGAME:
            m_worldView.reset(sf::FloatRect(0, 0, screen_Width, screen_Height));
            m_window.draw(m_gameMenu);
            break;
        default:
            break;
    }
    m_window.display();

}

void Game::checkCollisions()
{
    bool beatLevel = false;
    collisions.check(m_player, *m_goal, beatLevel);
    if (beatLevel)
    {
        loadNewLevel();
    }
    for (Enemy *m_enemy: m_zombies)
    {
        collisions.check(m_player, *m_enemy);
        collisions.checkNoiseCollision(*m_enemy);
    }
    for (Pickup *bottle: m_pickups)
    {
        if (!bottle->getCollected())
        {
            collisions.check(m_player, *bottle, m_hud);
        }
    }

    noiseCounter++;
    for (auto &object: m_environment)
    {
        collisions.check(m_player, object, noiseCounter);
    }
}


void Game::setupBase()
{
    if (!m_spriteSheet.loadFromFile("ASSETS/TileSheet/spritesheet.png"))
        std::cout << "problem loading the game sprite sheet" << std::endl;

    m_spriteSheet.setSmooth(true);
    std::ifstream levelData("scene.json");
    json groundJson;

    groundJson = json::parse(levelData);

    json scene = groundJson["scene"];

    std::ifstream spriteSheetData("ASSETS/TileSheet/spritesheet.json");
    nlohmann::json json;
    spriteSheetData >> json;

    nlohmann::json frame = json["frames"];

    // Build scene from the json file
    for (auto &el: scene)
        m_ground.emplace_back(m_spriteSheet, el["spriteName"], el["gridIndex"], screen_Height / tileSize,
                              screen_Width / tileSize, frame, el["rotation"]);
}

void Game::setupEnvironment()
{
    std::ifstream levelData("level" + std::to_string(level) + ".json");
    json levelJson;
    levelJson = json::parse(levelData);
    json scene = levelJson["scene"];

    std::ifstream spriteSheetData("ASSETS/TileSheet/spritesheet.json");

    nlohmann::json json;
    spriteSheetData >> json;

    nlohmann::json frame = json["frames"];

    m_environment.clear();
    for (auto &el: scene)
        m_environment.emplace_back(m_spriteSheet, el["spriteName"], el["gridIndex"], screen_Height / tileSize,
                                   screen_Width / tileSize, frame, el["rotation"], el["impassable"]);

    for (auto &object: m_environment)
    {
        auto node = m_grid.nodeIndex(object.getTileCode());
        // Update the node
        node->m_data.isPassable = !object.isImpassable();
        m_grid.updateNode(node->m_data, node->m_data.id);
    }
    setUpSpecial(levelJson);
}

void Game::cameraMovement(sf::Time dt)
{
    sf::Vector2f movement = m_player.getPosition() - m_worldView.getCenter();
    m_worldView.move(movement * dt.asSeconds());
}

int Game::cellIdFinder(sf::Vector2f t_targetLocation)
{
    int m_id = floor(t_targetLocation.x / tileSize) + (floor(t_targetLocation.y / tileSize) * m_gridCols);
    return m_id;
}

void Game::setUpSpecial(json &levelData)
{
    json keyElements = levelData["special"];
    m_zombies.clear();
    m_pickups.clear();

    for (auto &el: keyElements)
    {
        if (el["Type"] == "Player")
        {
            m_player.setupNewPlayer(el["gridIndex"], el["rotation"]);
        } else if (el["Type"] == "Zombie")
        {
            m_zombies.emplace_back(new Enemy(el["gridIndex"], el["rotation"], m_grid, m_sounds));
        } else if (el["Type"] == "Pickup")
        {
            m_pickups.emplace_back(new Pickup(el["gridIndex"], m_spriteSheet));
        } else if (el["Type"] == "Goal")
        {
            m_goal = new Goal(el["gridIndex"]);
        }
    }
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

void Game::loadNewLevel()
{
    m_worldView.reset(sf::FloatRect(0, 0, screen_Width, screen_Height));
    m_window.clear(sf::Color::Black);
    m_window.setView(m_worldView);
    m_window.draw(m_loadingText);
    m_window.display();
    level++;
    std::ifstream f("level" + std::to_string(level) + ".json");
    if (f.good())
    {
        collisions.clearNoises();
        setupEnvironment();
    } else
    {
        m_gameState = GameState::WINGAME;
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

    // Growling
    m_sounds.load(Sounds::Zombie_Growling_1, path + "/Growling/Zombie_1.ogg");
    m_sounds.load(Sounds::Zombie_Growling_2, path + "/Growling/Zombie_2.ogg");
    m_sounds.load(Sounds::Zombie_Growling_3, path + "/Growling/Zombie_3.ogg");
    m_sounds.load(Sounds::Zombie_Growling_4, path + "/Growling/Zombie_4.ogg");
    m_sounds.load(Sounds::Zombie_Growling_5, path + "/Growling/Zombie_5.ogg");
    m_sounds.load(Sounds::Zombie_Growling_6, path + "/Growling/Zombie_6.ogg");
    m_sounds.load(Sounds::Zombie_Growling_7, path + "/Growling/Zombie_7.ogg");
    m_sounds.load(Sounds::Zombie_Growling_8, path + "/Growling/Zombie_8.ogg");
    m_sounds.load(Sounds::Zombie_Growling_9, path + "/Growling/Zombie_9.ogg");

    m_sounds.load(Sounds::Wine_Glass_Shatering, path + "/Effects/Wine_Glass_Shatering.ogg");
}

void Game::openMusicFiles()
{
    std::string path = "ASSETS/MUSICS";

    if (!m_menuMusic.openFromFile(path + "/Menu.ogg"))
        std::cout << "Error opening menu music file" << std::endl;

    if (!m_inGameMusic.openFromFile(path + "/InGameMusic.ogg"))
        std::cout << "Error opening in game music file" << std::endl;
}
