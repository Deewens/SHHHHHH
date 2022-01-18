/// <summary>
///
/// </summary>

#include <algorithm>

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
        m_window(sf::VideoMode{screen_Width, screen_Height, 32U}, "SHHHH...!"),
        m_exitGame(false), //when true game will exit
        m_spawnPosition(100.f, 100.f),
        m_ucsWaypoints({
                               85, 95, // 1, 2
                               205 // 3
                       }),
        m_grid(Graph<NodeData, float>(screen_Height / tileSize, screen_Width / tileSize, 300))
{

    if (!m_font.loadFromFile("ASSETS\\FONTS\\ariblk.ttf"))
    {
        std::cout << "Problem loading font" << std::endl;
    }

    m_playerCoordsDebugText.setFont(m_font);
    m_playerCoordsDebugText.setCharacterSize(20);
    m_playerCoordsDebugText.setFillColor(sf::Color::White);
    m_playerCoordsDebugText.setPosition(0, 0);

    loadSounds();
    m_player.loadSoundHolder(m_sounds);
    m_enemy.loadSoundHolder(m_sounds);
    m_ucsEnemy.loadSoundHolder(m_sounds);

    m_gameMenu.Init();

    m_ucsEnemy.setPosition(330, 270);

    pauseMenuSetUp();

    setupEnvironment();
    setUpPickUps();

    m_worldView.reset(
            sf::FloatRect(m_player.getPosition().x, m_player.getPosition().y, screen_Width / 2, screen_Height / 2));
    m_menuView.reset(sf::FloatRect(0, 0, screen_Width, screen_Height));
    m_grid.debug();

    // Fill the graph with all the arcs using the neighbours algorithm
    for (int i = 0; i < m_grid.getNodes().size(); i++)
    {
        std::map<int, float> neighbours = m_grid.getNeighbours(i);
        for (auto neighbour: neighbours)
        {
            m_grid.addArc(i, neighbour.first, neighbour.second);
        }
    }

/*    std::vector<Node*> path1;
    m_grid.ucs(m_grid.nodeIndex(85), m_grid.nodeIndex(95), visit, path1);
    m_grid.clearMarks();
    std::vector<Node*> path2;
    m_grid.ucs(m_grid.nodeIndex(95), m_grid.nodeIndex(85), visit, path2);*/

    //m_grid.ucs(m_grid.nodeIndex(95), m_grid.nodeIndex(85), visit, path2);

    for (int i = 0; i < m_ucsWaypoints.size(); i++)
    {
        for (int j = 0; j < m_ucsWaypoints.size(); j++)
        {
            if (m_ucsWaypoints[i] != m_ucsWaypoints[j])
            {
                std::vector<Node *> path;

                m_grid.ucs(m_grid.nodeIndex(m_ucsWaypoints[i]), m_grid.nodeIndex(m_ucsWaypoints[j]), visit, path);
                m_ucsPaths.insert({std::to_string(i) + "-" + std::to_string(j), path});
            }
            m_grid.clearMarks();
        }
    }

    m_path = m_ucsPaths.at("0-1");

/*    for (auto& neighbour : m_grid.getNeighbours(85))
    {
        std::cout << neighbour.first << " " << neighbour.second << std::endl;
    }*/

    for (auto& path : m_ucsPaths)
    {
        std::cout << "[" << path.first << "]: ";
        for (auto& node : path.second)
        {
            std::cout << node->m_data.id << " -> ";
        }
        std::cout << std::endl;
    }

    // TODO: To be deleted before merging into master
/*    m_grid.ucs(m_grid.nodeIndex(85), m_grid.nodeIndex(95), visit, m_path);
    for (auto it = m_path.rbegin(); it != m_path.rend(); ++it)
    {
        std::cout << (*it)->m_data.id << " {" << (*it)->m_data.position.x << "," << (*it)->m_data.position.y << "} -> ";
    }
    std::cout << std::endl;*/

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
            m_gameMenu.update((sf::Vector2f) sf::Mouse::getPosition(m_window));
            break;
        case GameState::GAMEPLAY:
        {
            m_window.setView(m_worldView);
            m_hud.update(m_worldView.getCenter());
            m_player.update(t_deltaTime);
            m_enemy.update(t_deltaTime);
            m_ucsEnemy.update(t_deltaTime);
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
            volume = std::max<float>(0.f, 100.f - 100.f / 300.f * dist);
            m_enemy.changeSoundsVolume(volume);

            int zombieCell = floor(m_ucsEnemy.getPosition().x / tileSize) +
                             (floor(m_ucsEnemy.getPosition().y / tileSize) * screen_Width / tileSize);

            int secondToLastCell = 0;
            int lastCell = 0;

            if (m_path.size() > 1)
            {
                secondToLastCell = m_path[m_path.size() - 2]->m_data.id;
                if (secondToLastCell == zombieCell && m_path.size() > 2)
                {
                    m_path.erase(m_path.begin() + m_path.size() - 1);
                    secondToLastCell = m_path[m_path.size() - 2]->m_data.id;
                }
                lastCell = m_path[m_path.size() - 1]->m_data.id;

                int m_columS = secondToLastCell % (screen_Width / tileSize);
                int m_rowS = (secondToLastCell - m_columS) / (screen_Width / tileSize);

                int m_columF = lastCell % (screen_Width / tileSize);
                int m_rowF = (lastCell - m_columF) / (screen_Width / tileSize);

                sf::Vector2f m_centerS;
                m_centerS.y = (tileSize / 2) + (tileSize * m_rowS);
                m_centerS.x = (tileSize / 2) + (tileSize * m_columS);

                sf::Vector2f m_centerF;
                m_centerF.y = (tileSize / 2) + (tileSize * m_rowF);
                m_centerF.x = (tileSize / 2) + (tileSize * m_columF);

                m_ucsEnemy.move(m_centerS, m_centerF);
            }
        }
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

            for (Environment env: m_environment)
            {
                env.render(m_window);
            }

            for (auto &object: m_objects)
            {
                object.render(m_window);
            }

            for (int i = 0; i < 2; i++)
            {
                if (!m_pickupCollected[i])
                {
                    m_window.draw(*m_pickup[i]);
                }
            }
            m_window.draw(m_player);
            m_window.draw(m_enemy);
            m_window.draw(m_ucsEnemy);
            m_enemy.renderVisionCone(m_window);
            m_window.draw(m_grid);
            collisions.renderNoises(m_window);
            m_player.renderPowerBar(m_window);
            m_hud.render(m_window);

            m_window.setView(m_window.getDefaultView());
            m_window.draw(m_playerCoordsDebugText);
            break;
        case GameState::EXIT:
            break;
        case GameState::OPTIONS:
            m_window.draw(m_gameMenu);
            break;
        case GameState::PAUSE:
            m_window.setView(m_worldView);
            for (Environment env: m_environment)
            {
                env.render(m_window);
            }

            for (auto &object: m_objects)
            {
                object.render(m_window);
            }

            m_window.draw(m_player);
            //m_window.draw(m_pickup);
            m_window.draw(m_enemy);
            m_window.draw(m_ucsEnemy);
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

    for (auto &object: m_objects)
    {
        if (object.isImpassable()) collisions.check(m_player, object);
    }
}

void Game::pauseMenuSetUp()
{
    m_pauseRect.setSize(sf::Vector2f(screen_Width / 4, screen_Height / 4));
    m_pauseRect.setFillColor(sf::Color(225, 0, 0, 100));
    m_pauseRect.setOrigin(screen_Width / 8, screen_Height / 8);
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
    for (auto &el: scene)
        m_environment.emplace_back(m_groundTexture, el["spriteName"], el["gridIndex"], screen_Height / tileSize,
                                   screen_Width / tileSize, el["rotation"]);


    // Debug for UCS Pathfinding
    sf::RectangleShape waypointDebug;
    waypointDebug.setFillColor(sf::Color::Black);
    for (auto &w: m_ucsWaypoints)
        m_objects.emplace_back(waypointDebug, w, screen_Height / tileSize, screen_Width / tileSize, 0, false);

    // Add impassable wall for UCS Debugging
    // ===========================================================================
    sf::RectangleShape wall;
    wall.setFillColor(sf::Color::Red);
    m_objects.emplace_back(wall, 70, screen_Height / tileSize, screen_Width / tileSize, 0, true);
    m_objects.emplace_back(wall, 71, screen_Height / tileSize, screen_Width / tileSize, 0, true);
    m_objects.emplace_back(wall, 72, screen_Height / tileSize, screen_Width / tileSize, 0, true);
    m_objects.emplace_back(wall, 73, screen_Height / tileSize, screen_Width / tileSize, 0, true);
    m_objects.emplace_back(wall, 73, screen_Height / tileSize, screen_Width / tileSize, 0, true);
    m_objects.emplace_back(wall, 74, screen_Height / tileSize, screen_Width / tileSize, 0, true);
    m_objects.emplace_back(wall, 90, screen_Height / tileSize, screen_Width / tileSize, 0, true);
    m_objects.emplace_back(wall, 94, screen_Height / tileSize, screen_Width / tileSize, 0, true);
    m_objects.emplace_back(wall, 114, screen_Height / tileSize, screen_Width / tileSize, 0, true);
    m_objects.emplace_back(wall, 134, screen_Height / tileSize, screen_Width / tileSize, 0, true);
    m_objects.emplace_back(wall, 154, screen_Height / tileSize, screen_Width / tileSize, 0, true);
    m_objects.emplace_back(wall, 174, screen_Height / tileSize, screen_Width / tileSize, 0, true);
    m_objects.emplace_back(wall, 173, screen_Height / tileSize, screen_Width / tileSize, 0, true);
    m_objects.emplace_back(wall, 172, screen_Height / tileSize, screen_Width / tileSize, 0, true);
    m_objects.emplace_back(wall, 171, screen_Height / tileSize, screen_Width / tileSize, 0, true);
    m_objects.emplace_back(wall, 170, screen_Height / tileSize, screen_Width / tileSize, 0, true);
    m_objects.emplace_back(wall, 150, screen_Height / tileSize, screen_Width / tileSize, 0, true);
    m_objects.emplace_back(wall, 130, screen_Height / tileSize, screen_Width / tileSize, 0, true);
    m_objects.emplace_back(wall, 110, screen_Height / tileSize, screen_Width / tileSize, 0, true);
    // ===========================================================================

    // Set the passable property for each grid node
    for (auto &node: m_grid.getNodes())
    {
        for (auto &object: m_objects)
        {
            if (object.getTileCode() == node->m_data.id)
            {
                // Update the node
                NodeData newData = node->m_data;
                newData.isPassable = !object.isImpassable();
                m_grid.updateNode(newData, node->m_data.id);
            }
        }

        //std::cout << "Is " << node->m_data.id << " passable: " << (node->m_data.isPassable ? "Yes" : "No") << std::endl;
    }
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
    m_pickup[0] = new Pickup(22);
    m_pickup[1] = new Pickup(43);
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




