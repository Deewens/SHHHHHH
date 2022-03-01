/// <summary>
/// @author Peter Lowe
/// @date May 2019
///
/// you need to change the above lines or lose marks
/// </summary>

#include "Game.h"
#include <iostream>

void visit(Node* t_node)
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
	//m_window{ sf::VideoMode{ screen_Width + menu_Width, screen_Height, 32U }, "LevelLoader" },
	m_window{ sf::VideoMode{ sf::VideoMode::getDesktopMode().width, sf::VideoMode::getDesktopMode().height, 32U}, "LevelLoader"},
	m_exitGame{false}, //when true game will exit
	view(m_window.getDefaultView()),
	m_graph(Graph<NodeData, float>(screen_Height / tileSize, screen_Width / tileSize, 300))
{

	sf::View v = m_window.getDefaultView();
	m_window.setView(v);

	calculateUcsWaypoints();

	setupSprite(); // load texture
	setupHUD();

	if (!m_font.loadFromFile("ASSETS/FONTS/ariblk.ttf"))
		std::cout << "Error when loading the ariblk font" << std::endl;
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

		if (sf::Event::Resized == newEvent.type)
		{
			sf::FloatRect visibleArea(0, 0, newEvent.size.width, newEvent.size.height);
			sf::View v(visibleArea);
			m_window.setView(v);
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

	if (t_event.key.code == sf::Keyboard::D)
	{
		m_graph.toggleDraw();
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
	for (Button* button : tileOptions[currentCategory])
	{
		button->render(m_window);
	}
	m_window.draw(topScrollBlock);
	m_window.draw(bottomScrollBlock);
	saveButton->render(m_window);
	deleteButton->render(m_window);
	rotateButton->render(m_window);
	upButton->render(m_window);
	downButton->render(m_window);
	leftButton->render(m_window);
	rightButton->render(m_window);
	m_window.draw(currentCategoryText);
	for (int i = 0; i < mapSize; i++)
	{
		if (m_MapTiles[i] != nullptr)
		{
			m_MapTiles[i]->render(m_window);
		}
	}

	
	for (auto & sprite : m_ucsDebugTiles)
	{
		m_window.draw(sprite);
	}

	m_window.draw(m_graph);
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
}

void Game::setupHUD()
{
	if (!m_texture.loadFromFile("ASSETS/IMAGES/spritesheet.png"))
	{

	}
	saveButton = new Button(sf::Vector2f(screen_Width, screen_Height - 100), sf::Vector2f(menu_Width, 100), sf::Color::Green, "Save", 40, sf::Color::Black);
	deleteButton = new Button(sf::Vector2f(screen_Width, screen_Height - 160), sf::Vector2f(menu_Width / 2, 60), sf::Color::Red, 
		"Delete", 30, sf::Color::Black);
	rotateButton = new Button(sf::Vector2f(screen_Width + menu_Width / 2, screen_Height - 160), sf::Vector2f(menu_Width / 2, 60), sf::Color::Blue,
		"Rotate", 30, sf::Color::Black);
	upButton = new Button(sf::Vector2f(screen_Width + (3 * menu_Width / 8), tileListTop - 90), sf::Vector2f(menu_Width / 4, 50), sf::Color::Cyan,
		"^", 30, sf::Color::Black);
	downButton = new Button(sf::Vector2f(screen_Width + (3 * menu_Width / 8), tileListBottom + 30), sf::Vector2f(menu_Width / 4, 50), sf::Color::Cyan,
		"v", 30, sf::Color::Black);
	leftButton = new Button(sf::Vector2f(screen_Width, tileListTop - 170 ), sf::Vector2f(menu_Width / 8, 50), sf::Color::Cyan,
		"<", 30, sf::Color::Black);
	rightButton = new Button(sf::Vector2f(screen_Width + ((menu_Width*7) / 8), tileListTop - 170), sf::Vector2f(menu_Width / 8, 50), sf::Color::Cyan,
		">", 30, sf::Color::Black);
	
	if (!m_font.loadFromFile("ASSETS/FONTS/ariblk.ttf"))
	{

	}
	currentCategoryText.setPosition(screen_Width + menu_Width / 2, tileListTop - 150);
	currentCategoryText.setFont(m_font);
	currentCategoryText.setCharacterSize(24);
	currentCategoryText.setFillColor(sf::Color::Black);

	topScrollBlock.setPosition(screen_Width, 0);
	topScrollBlock.setSize(sf::Vector2f(menu_Width, tileListTop));
	bottomScrollBlock.setPosition(screen_Width, tileListBottom);
	bottomScrollBlock.setSize(sf::Vector2f(menu_Width, screen_Height));

	setupOptions();
}

void Game::setupOptions()
{
	/*
	Types:
		UI
		Player
		Beach
		Indoor_Ground
		Outdoor_Ground
		River
		Green_Zombie
		Purple_Zombie
		Indoor_Decor
		Outdoor_Decor
		Wall
	*/
	bool playerAdded = false;
	bool zombieAdded = false;

	titles[0] = "Special";
	titles[1] = "Outdoor_Ground";
	titles[2] = "Indoor_Ground";
	titles[3] = "River";
	titles[4] = "Outdoor_Decor";
	titles[5] = "Indoor_Decor";
	titles[6] = "Wall";
	passable[0] = false;
	passable[1] = true;
	passable[2] = true;
	passable[3] = false;
	passable[4] = false;
	passable[5] = false;
	passable[6] = false;

	assignText();

	std::ifstream spriteSheetData("ASSETS/IMAGES/spritesheet.json");
	nlohmann::json json;
	spriteSheetData >> json;
	
	nlohmann::json frames = json["frames"];
	std::string category;
	int counter[NUM_CATEGORIES] = { 0 };
	Button* temp;
	sf::IntRect tempRect;
	for (nlohmann::json::iterator it = frames.begin(); it != frames.end(); it++)
	{
		auto& el = it.value();
		category = el["Type"];
		bool found = false;
		for (int i = 1; i < NUM_CATEGORIES; i++)
		{
			if (category == titles[i])
			{
				tempRect = sf::IntRect(el["frame"]["x"], el["frame"]["y"], el["frame"]["w"], el["frame"]["h"]);
				temp = new Button(sf::Vector2f(screen_Width + (menu_Width / 2 - 75), tileListTop + (200 * counter[i])), 
					sf::Vector2f(150, 150), m_texture, tempRect, passable[i], it.key(), false);
				tileOptions[i].push_back(temp);
				found = true;
				counter[i]++;
			}
		}
		if (!found)
		{
			if (category == "UI" && it.key() == "Bottle.png")
			{
				tempRect = sf::IntRect(el["frame"]["x"], el["frame"]["y"], el["frame"]["w"], el["frame"]["h"]);
				temp = new Button(sf::Vector2f(screen_Width + (menu_Width / 2 - 75), tileListTop + (200 * counter[0])),
					sf::Vector2f(150, 150), m_texture, tempRect, passable[0], it.key(), true);
				tileOptions[0].push_back(temp);
				counter[0]++;
			}
			else if (!playerAdded && category == "Player")
			{
				tempRect = sf::IntRect(el["frame"]["x"], el["frame"]["y"], el["frame"]["w"], el["frame"]["h"]);
				temp = new Button(sf::Vector2f(screen_Width + (menu_Width / 2 - 75), tileListTop + (200 * counter[0])),
					sf::Vector2f(150, 150), m_texture, tempRect, passable[0], it.key(), true);
				tileOptions[0].push_back(temp);
				counter[0]++;
				playerAdded = true;
			}
			else if (!zombieAdded && category == "Green_Zombie")
			{
				tempRect = sf::IntRect(el["frame"]["x"], el["frame"]["y"], el["frame"]["w"], el["frame"]["h"]);
				temp = new Button(sf::Vector2f(screen_Width + (menu_Width / 2 - 75), tileListTop + (200 * counter[0])),
					sf::Vector2f(150, 150), m_texture, tempRect, passable[0], it.key(), true);
				tileOptions[0].push_back(temp);
				counter[0]++;
				zombieAdded = true;
			}
		}
	}
}

void Game::manageClicks(sf::Event t_event)
{
	sf::Vector2i click = sf::Mouse::getPosition(m_window);
	sf::Vector2f worldPosClick = m_window.mapPixelToCoords(click);
	
	if (t_event.type == sf::Event::MouseButtonPressed && t_event.mouseButton.button == sf::Mouse::Right)
	{
		isRightBtnPressed = true;
		oldClickPos = sf::Vector2f(sf::Mouse::getPosition(m_window));
	}
	else if (t_event.type == sf::Event::MouseButtonReleased && t_event.mouseButton.button == sf::Mouse::Right)
	{
		isRightBtnPressed = false;
	}
	else if (t_event.type == sf::Event::MouseMoved)
	{
		// Prevent the bug if the mouse button is pressed and released to quickly
		// If it happens, the btn isRightBtnPressed is not updated accordignly
		if (!sf::Mouse::isButtonPressed(sf::Mouse::Button::Right)) isRightBtnPressed = false;

		if (isRightBtnPressed)
		{
			sf::Vector2f newPos = sf::Vector2f(sf::Mouse::getPosition(m_window));
			sf::Vector2f deltaPos = oldClickPos - newPos;

			view.move(deltaPos);
			m_window.setView(view);
			oldClickPos = newPos;

		}
	}

	//holding down mouse
	if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
	{
		if (upButton->isInside(worldPosClick))
		{
			for (Button* button : tileOptions[currentCategory])
			{
				button->moveUp(2.0f);
			}
		}
		if (downButton->isInside(worldPosClick))
		{
			for (Button* button : tileOptions[currentCategory])
			{
				button->moveDown(2.0f);
			}
		}
	}
	if (t_event.type == sf::Event::MouseButtonReleased && t_event.mouseButton.button == sf::Mouse::Left)
	{
		//check if you interacted with the ui
		if (worldPosClick.x >= screen_Width && worldPosClick.x < screen_Width + menu_Width &&
			worldPosClick.y >= 0 && worldPosClick.y <= screen_Height)
		{
			if (worldPosClick.y > tileListTop && worldPosClick.y <= tileListBottom)
			{
				for (Button* button : tileOptions[currentCategory])
				{
					if (button->isInside(worldPosClick))
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
						else if (isRotating)
						{
							isRotating = false;
							rotateButton->setSelected(false);
						}
					}
				}
			}
			else if (deleteButton->isInside(worldPosClick))
			{
				isDeleting = true;
				deleteButton->setSelected(true);
				isRotating = false;
				rotateButton->setSelected(false);
				if (selectedButton != nullptr)
				{
					selectedButton->setSelected(false);
					selectedButton = nullptr;
				}
			}
			else if (rotateButton->isInside(worldPosClick))
			{
				isDeleting = false;
				deleteButton->setSelected(false);
				isRotating = true;
				rotateButton->setSelected(true);
				if (selectedButton != nullptr)
				{
					selectedButton->setSelected(false);
					selectedButton = nullptr;
				}
			}
			else if (leftButton->isInside(worldPosClick))
			{
				if (currentCategory > 0)
				{
					currentCategory--;
					assignText();
				}
			}
			else if (rightButton->isInside(worldPosClick))
			{
				if (currentCategory < (NUM_CATEGORIES - 1))
				{
					currentCategory++;
					assignText();
				}
			}
			else if (saveButton->isInside(worldPosClick))
			{
				for (int i = 0; i < mapSize; i++)
				{
					if (m_MapTiles[i] != nullptr)
					{
						auto node = m_graph.nodeIndex(i);
						node->m_data.isPassable = selectedButton->getPassable();
						m_graph.updateNode(node->m_data, i);
					}
				}

				// Fill the graph with all the arcs using the neighbours algorithm
				for (int i = 0; i < m_graph.getNodes().size(); i++)
				{
					std::map<int, float> neighbours = m_graph.getNeighbours(i);
					for (auto neighbour : neighbours)
					{
						m_graph.addArc(i, neighbour.first, neighbour.second);
					}
				}

				// Calculate UCS Pathfinding for each between each waypoints
				for (int i = 0; i < m_waypointsIdx.size(); i++)
				{
					for (int j = 0; j < m_waypointsIdx.size(); j++)
					{
						if (m_waypointsIdx[i] != m_waypointsIdx[j])
						{
							std::vector<Node*> path;

							m_graph.ucs(m_graph.nodeIndex(m_waypointsIdx[i]), m_graph.nodeIndex(m_waypointsIdx[j]), visit, path);
							m_ucsPaths.insert({ std::to_string(m_graph.nodeIndex(m_waypointsIdx[i])->m_data.id) + "-" + std::to_string(m_graph.nodeIndex(m_waypointsIdx[j])->m_data.id), path });
						}
						m_graph.clearMarks();
					}
				}

				//int rows = screen_Height / tileSize;
				//int cols = screen_Width / tileSize;

				//m_ucsDebugTiles.clear();
				//for (auto& path : m_ucsPaths)
				//{
				//	for (auto& node : path.second)
				//	{
				//		sf::RectangleShape rect;
				//		rect.setFillColor(sf::Color(255, 255, 0, 64));
				//		rect.setSize(sf::Vector2f(tileSize, tileSize));
				//		rect.setOrigin(sf::Vector2f(tileSize / 2, tileSize / 2));
				//		float col = node->m_data.id % cols;
				//		float row = (node->m_data.id - col) / cols;
				//		col = (col * tileSize) + (tileSize / 2);
				//		row = (row * tileSize) + (tileSize / 2);
				//		rect.setPosition(sf::Vector2f(col, row));
				//		m_ucsDebugTiles.push_back(rect);
				//	}
				//}
				bool found = false;;

				std::string output = "{\n	\"scene\": [";
				for (int i = 0; i < mapSize; i++)
				{
					if (m_MapTiles[i] != nullptr)
					{
						output += m_MapTiles[i]->getJsonInfo(i);
						found = true;
					}
				}
				if (found)
				{
					output.pop_back();
					found = false;
				}
				output += "\n],";
				output += "\n\"special\": [";;
				
				for (int i = 0; i < mapSize; i++)
				{
					if (m_MapTiles[i] != nullptr)
					{
						output += m_MapTiles[i]->getSpecialJson(i);
						found = true;
					}
				}
				if (found)
				{
					output.pop_back();
				}
				
				output += "\n],";
				

				output += "\n\"pathfinding\": {";
				output += "\t\"waypoints\": [";
				for (int i = 0; i < m_waypointsIdx.size(); i++)
				{
					output += "\n" + std::to_string(m_waypointsIdx[i]);
					if (i < m_waypointsIdx.size() - 1) output += ", ";
				}
				output += "],";
				output += "\n\"paths\": {";
				for (auto& path : m_ucsPaths)
				{
					output += "\"" + path.first + "\": [";
					// For loop count down because the calculated path is inverted
					for (int i = path.second.size() - 1; i != -1; i--)
					{
						auto& node = path.second[i];
						output += "\n" + std::to_string(node->m_data.id);
						if (i != 0) output += ", ";
					}
					output += "],";
				}
				output.pop_back();
				output += "\n}";
				output += "\n}";

				output += "\n}";
				std::cout << output << std::endl;
				std::ofstream MyFile("C://Users//gameuser//Desktop//Newfolder//project21-masih-adrien-and-eoin//Year3-Project//Year3-Project//level.json");

				// Write to the file
				MyFile << output;

				// Close the file
				MyFile.close();
			}
		}
		//check if you clicked on the map
		else if (worldPosClick.x <= screen_Width && worldPosClick.x > 0 &&
			worldPosClick.y >= 0 && worldPosClick.y <= screen_Height)
		{
			int tileNum = trunc(worldPosClick.x / tileSize) + (trunc(worldPosClick.y / tileSize) * (screen_Width / tileSize));
			std::cout << tileNum << std::endl;
			auto result = std::find_if(m_waypointsIdx.begin(), m_waypointsIdx.end(), [=](int val) {return val == tileNum; });

			bool isUcsWaypoint = false;
			if (result != m_waypointsIdx.end())
				isUcsWaypoint = true;

			if (isDeleting)
			{
				m_MapTiles[tileNum] = nullptr;
			}
			else if (isRotating && m_MapTiles[tileNum] != nullptr)
			{
				m_MapTiles[tileNum]->rotate();
			}
			else if (selectedButton != nullptr)
			{
				if (m_MapTiles[tileNum] == nullptr)
				{
					if (isUcsWaypoint && !selectedButton->getPassable())
					{
						std::cout << "You can't place an impassable tile here." << std::endl;
					}
					else
					{
						m_MapTiles[tileNum] = new Tile(selectedButton, tileNum, m_texture);

					}
				}
				else
				{
					m_MapTiles[tileNum]->changeLayers(selectedButton);
				}
			}
		}
	}
}

void Game::assignText()
{
	currentCategoryText.setString(titles[currentCategory]);
	currentCategoryText.setOrigin(currentCategoryText.getGlobalBounds().width / 2, currentCategoryText.getGlobalBounds().height / 2);
}

/// <summary>
/// Calculate the position of each waypoints for the UCS pathfinding calculation.
/// Store them in the m_ucsWaypointsIdx member property
/// </summary>
void Game::calculateUcsWaypoints()
{
	float gridWidth = screen_Width / tileSize;
	float gridHeight = screen_Height / tileSize;

	// Calculate the central and the fourth edge coords
	sf::Vector2f center{ gridWidth / 2, gridHeight / 2 }; // Calculate the central node index
	sf::Vector2f topLeftCenter{ center.x / 2, center.y / 2 };
	sf::Vector2f bottomLeftCenter{ topLeftCenter.x, topLeftCenter.y * 3 };
	sf::Vector2f topRightCenter{ topLeftCenter.x * 3, topLeftCenter.y };
	sf::Vector2f bottomRightCenter{ topLeftCenter.x * 3, topLeftCenter.y * 3 };

	// Calculate the index number for the center and the fourth edge
	// Formula: (y * gridWidth) + x
	int centerIdx = trunc(center.x) + (trunc(center.y) * gridWidth);
	int topLeftIdx = trunc(topLeftCenter.x) + (trunc(topLeftCenter.y) * gridWidth);
	int bottomLeftIdx = trunc(bottomLeftCenter.x) + (trunc(bottomLeftCenter.y) * gridWidth);
	int topRightIdx = trunc(topRightCenter.x) + (trunc(topRightCenter.y) * gridWidth);
	int bottomRightIdx = trunc(bottomRightCenter.x) + (trunc(bottomRightCenter.y) * gridWidth);

	std::cout << topLeftIdx << " - " << topRightIdx << std::endl;
	std::cout << "   " << centerIdx << std::endl;
	std::cout << bottomLeftIdx << " - " << bottomRightIdx << std::endl;

	m_waypointsIdx.push_back(topLeftIdx);
	m_waypointsIdx.push_back(topRightIdx);
	m_waypointsIdx.push_back(centerIdx);
	m_waypointsIdx.push_back(bottomLeftIdx);
	m_waypointsIdx.push_back(bottomRightIdx);

	m_graph.debug();
}
