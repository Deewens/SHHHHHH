#include "Enemy.h"

Enemy::Enemy(int t_gridIndex, int rotation, Graph<NodeData, float>& t_grid, SoundHolder& soundHolder):
    m_runningAnim(m_sprite),
    m_idlingAnim(m_sprite),
    m_grid(t_grid)
{
    Enemy::loadTexture();
    switch (rotation)
    {
    case 0:
        Enemy::setDirection(EAST);
        break;
    case 90:
        Enemy::setDirection(SOUTH);
        break;
    case 180:
        Enemy::setDirection(WEST);
        break;
    case 270:
        Enemy::setDirection(NORTH);
        break;
    }

    loadSoundHolder(soundHolder);

    std::ifstream spriteSheetData("ASSETS/IMAGES/sprite_sheets/data/characters_sprite_sheet.json");
    nlohmann::json json;
    spriteSheetData >> json;

    for (auto& frames : json["frames"])
    {
        std::string filename = frames["filename"];

        std::string::size_type idlingFound = filename.find("zombies/purple/idling/");
        std::string::size_type runningFound = filename.find("zombies/purple/running");

        if (idlingFound != std::string::npos)
        {
            nlohmann::json frame = frames["frame"];
            int x = frame["x"];
            int y = frame["y"];
            int width = frame["w"];
            int height = frame["h"];

            m_idlingAnim.addFrame({ sf::IntRect(x, y, width, height) });
        }
        else if (runningFound != std::string::npos)
        {
            nlohmann::json frame = frames["frame"];
            int x = frame["x"];
            int y = frame["y"];
            int width = frame["w"];
            int height = frame["h"];

            m_runningAnim.addFrame({ sf::IntRect(x, y, width, height), 0.05 });
        }
    }

    float col = t_gridIndex % (screen_Width / tileSize);
    float row = (t_gridIndex - col) / (screen_Width / tileSize);
    col = (col * tileSize) + (tileSize / 2);
    row = (row * tileSize) + (tileSize / 2);

    m_sprite.setOrigin(tileSize / 2, tileSize / 2);
    m_sprite.setPosition(col, row);

    m_EnemyState = EnemyState::SEEK;

    m_searchCounter = 500;

    m_path.setPrimitiveType(sf::LineStrip);

}

void Enemy::setDirection(int t_direction)
{   
    switch (t_direction)
    {
    case NORTH:
        m_sprite.setRotation(270);
        break;
    case SOUTH:
        m_sprite.setRotation(90);
        break;
    case EAST:
        m_sprite.setRotation(0);
        break;
    case WEST:
        m_sprite.setRotation(180);
        break;
    case NORTHWEST:
        m_sprite.setRotation(225);
        break;
    case NORTHEAST:
        m_sprite.setRotation(315);
        break;
    case SOUTHWEST:
        m_sprite.setRotation(135);
        break;
    case SOUTHEAST:
        m_sprite.setRotation(45);
        break;
    }
}

void Enemy::update(sf::Time deltaTime)
{
    sf::Time elapsed = clock.getElapsedTime();

    if (m_EnemyState == EnemyState::SEEK)
    {
        setVisionCone(30);
    }
    if (m_EnemyState == EnemyState::ATTACK)
    {
        setVisionCone(20);
    }
    if (isBeingSeen())
    {
        m_directionEnd = m_playerLocation;
        move(m_playerLocation, m_sprite.getPosition());
        m_EnemyState = EnemyState::ATTACK;
        m_runningAnim.update(deltaTime.asSeconds());
        if (elapsed.asSeconds() >= 0.5)
        {
            Utils::playRandomSound(footstepRunSounds);
            clock.restart();
        }
    }
    else
    {
        m_EnemyState = EnemyState::SEEK;
        m_idlingAnim.update(deltaTime.asSeconds());
    }
    //debug();

    if (elapsed.asSeconds() >= 5)
    {
        Utils::playRandomSound(growlingSounds);
        clock.restart();
    }
    pathFinding();
}

void Enemy::renderVisionCone(sf::RenderWindow& t_window)
{
     t_window.draw(coneVision);


     sf::Vertex line[] =
     {
         m_directionLine0,
         m_directionLine1
     };
     t_window.draw(line, 2, sf::Lines);
     t_window.draw(m_playerCircle);
}

void Enemy::visionConeCollisionCheck(sf::Vector2f t_playerLocation)
{
    m_playerLocation = t_playerLocation;
}

void Enemy::setVisionCone(float t_angle)
{
    coneVision = sf::VertexArray(sf::Triangles, 3);
    m_visionP1 = m_sprite.getPosition();

    coneVision[0].position = m_visionP1;
    coneVision[1].position = m_visionP2;
    coneVision[2].position = m_visionP3;

    if (m_EnemyState == EnemyState::SEEK)
    {
        m_visionP1 = m_sprite.getPosition();
        m_visionP2 = m_sprite.getPosition() + (VISION_CONE_LENGTH_SEEK * rotatedVector(m_visionConeDir, -t_angle + m_sprite.getRotation()));
        m_visionP3 = m_sprite.getPosition() + (VISION_CONE_LENGTH_SEEK * rotatedVector(m_visionConeDir, t_angle + m_sprite.getRotation()));   

        coneVision[0].color = sf::Color::Green;
        coneVision[1].color = sf::Color::Transparent;
        coneVision[2].color = sf::Color::Transparent;

    }
    if (m_EnemyState == EnemyState::ATTACK)
    {
        m_visionP1 = m_sprite.getPosition();
        m_visionP2 = m_sprite.getPosition() + (VISION_CONE_LENGTH_ATTACK * rotatedVector(m_visionConeDir, -t_angle + m_sprite.getRotation()));
        m_visionP3 = m_sprite.getPosition() + (VISION_CONE_LENGTH_ATTACK * rotatedVector(m_visionConeDir, t_angle + m_sprite.getRotation()));

        coneVision[0].color = sf::Color::Red;
        coneVision[1].color = sf::Color::Transparent;
        coneVision[2].color = sf::Color::Transparent;
    }
}

sf::Vector2f Enemy::rotatedVector(const sf::Vector2f& vector, float t_angle)
{
    // No assert here, because turning a zero vector is well-defined (yields always zero vector)

    sf::Vector2f copy = vector;
    rotate(copy, t_angle);
    return copy;
}

void Enemy::rotate(sf::Vector2f& vector, float t_angle)
{
    // No assert here, because turning a zero vector is well-defined (yields always zero vector)

   float cos = std::cos(t_angle*M_PI/180);
   float sin = std::sin(t_angle*M_PI/180);
    // Don't manipulate x and y separately, otherwise they're overwritten too early
    vector = sf::Vector2f(
        cos * vector.x - sin * vector.y,
        sin * vector.x + cos * vector.y);
}
void Enemy::move(sf::Vector2f t_startVec, sf::Vector2f t_finishVec)
{
    sf::Vector2f m_movement = t_startVec - t_finishVec;

    float m_angleRad = atan2(m_movement.y, m_movement.x);

    int m_angleInDeg = (m_angleRad * 180.0f) / M_PI;

    int intermediaryAngle = m_angleInDeg % 45;

    m_angleInDeg -= intermediaryAngle;

    if (abs(intermediaryAngle) > 22)
    {
        if (intermediaryAngle > 0)
        {
            m_angleInDeg += 45;
        }
        else
        {
            m_angleInDeg -= 45;
        }
    }

    m_sprite.setRotation(m_angleInDeg);

    m_angleRad = m_angleInDeg * M_PI / 180;
    m_movement = sf::Vector2f(cos(m_angleRad), sin(m_angleRad));
    
    unitVector(m_movement);

    m_movement = ENEMY_SPEED * m_movement;  

    m_sprite.move(m_movement);
}

bool Enemy::isBeingSeen()
{
    circle_player.r = 17.0f;

    poly_visionCone.count = 3;

    poly_visionCone.verts[0] = c2V(m_visionP1.x, m_visionP1.y);
    poly_visionCone.verts[1] = c2V(m_visionP2.x, m_visionP2.y);
    poly_visionCone.verts[2] = c2V(m_visionP3.x, m_visionP3.y);

    circle_player.p.x = m_playerLocation.x;
    circle_player.p.y = m_playerLocation.y;

    int result = c2CircletoPoly(circle_player, &poly_visionCone, NULL);

    return result != 0;
}
void Enemy::unitVector(sf::Vector2f& t_vector)
{
    float length = sqrt((t_vector.x * t_vector.x) + (t_vector.y * t_vector.y));
    t_vector = t_vector / length;
}
void Enemy::debug()
{
    m_playerCircle.setRadius(17.0f);
    m_playerCircle.setOrigin(17.0f, 17.0);
    m_playerCircle.setPosition(m_playerLocation);
    m_playerCircle.setFillColor(sf::Color::Yellow);

    if (isBeingSeen())
    {
        m_directionLine0 = m_playerLocation;
        m_directionLine1 = m_visionP1;
        std::cout << "SEEN" << std::endl;
    } else
    {
        std::cout << "NOT SEEN" << std::endl;

    }
}

void Enemy::setPath(int from, int to)
{
    auto ucsPaths = m_grid.getUCSPaths();
    m_ucsPath = ucsPaths.find(std::to_string(from) + "-" + std::to_string(to))->second;
}

void Enemy::pathFinding()
{
    if (isBeingSeen()) // When the zombie see the player, we clear the path because the zombie follow the player without pathfinding
    {
        m_isMoving = false;
        m_moveTo = sf::Vector2f(0, 0);
        m_ucsPath.clear();
    }

    int enemyPos = Utils::vectorToNode(getPosition());
    int targetPos = Utils::vectorToNode(m_moveTo);

    if (enemyPos == targetPos) // When it reaches the target position, we stop the movement
    {
        m_isMoving = false;
        m_moveTo = sf::Vector2f(0, 0);
        m_ucsPath.erase(m_ucsPath.begin());
    }

    if (!m_isMoving)
    {
        if (m_ucsPath.size() > 1)
        {
            m_isMoving = true;
            m_moveTo = m_grid.nodeIndex(m_ucsPath.at(1))->m_data.position;
        }
    }

    if (m_isMoving) move(m_moveTo, getPosition());
}

void Enemy::drawPath(std::vector<int> t_path)
{
    m_path.clear();
    for(int val: t_path)
    {
        float col = val % (screen_Width / tileSize);
        float row = (val - col) / (screen_Width / tileSize);
        col = (col * tileSize) + (tileSize / 2);
        row = (row * tileSize) + (tileSize / 2);
        m_path.append(sf::Vertex(sf::Vector2f(col, row), sf::Color::Red));
    };
    
}

void Enemy::loadSoundHolder(SoundHolder& soundHolder)
{
    footstepRunSounds.push_back(new sf::Sound(soundHolder.get(Sounds::Footsteps_Run_Sand1)));
    footstepRunSounds.push_back(new sf::Sound(soundHolder.get(Sounds::Footsteps_Run_Sand2)));
    footstepRunSounds.push_back(new sf::Sound(soundHolder.get(Sounds::Footsteps_Run_Sand3)));
    footstepRunSounds.push_back(new sf::Sound(soundHolder.get(Sounds::Footsteps_Run_Sand4)));
    footstepRunSounds.push_back(new sf::Sound(soundHolder.get(Sounds::Footsteps_Run_Sand5)));
    footstepRunSounds.push_back(new sf::Sound(soundHolder.get(Sounds::Footsteps_Run_Sand6)));
    footstepRunSounds.push_back(new sf::Sound(soundHolder.get(Sounds::Footsteps_Run_Sand7)));
    footstepRunSounds.push_back(new sf::Sound(soundHolder.get(Sounds::Footsteps_Run_Sand8)));
    std::for_each(footstepRunSounds.begin(), footstepRunSounds.end(), [](auto sound) { sound->setVolume(20); });

    growlingSounds.push_back(new sf::Sound(soundHolder.get(Sounds::Zombie_Growling_1)));
    growlingSounds.push_back(new sf::Sound(soundHolder.get(Sounds::Zombie_Growling_2)));
    growlingSounds.push_back(new sf::Sound(soundHolder.get(Sounds::Zombie_Growling_3)));
    growlingSounds.push_back(new sf::Sound(soundHolder.get(Sounds::Zombie_Growling_4)));
    growlingSounds.push_back(new sf::Sound(soundHolder.get(Sounds::Zombie_Growling_5)));
    growlingSounds.push_back(new sf::Sound(soundHolder.get(Sounds::Zombie_Growling_6)));
    growlingSounds.push_back(new sf::Sound(soundHolder.get(Sounds::Zombie_Growling_7)));
    growlingSounds.push_back(new sf::Sound(soundHolder.get(Sounds::Zombie_Growling_8)));
    growlingSounds.push_back(new sf::Sound(soundHolder.get(Sounds::Zombie_Growling_9)));
    std::for_each(growlingSounds.begin(), growlingSounds.end(), [](auto sound) { sound->setVolume(20); });
}

void Enemy::changeSoundsVolume(float newVolume)
{
    std::for_each(footstepRunSounds.begin(), footstepRunSounds.end(), [=](auto sound) {
        if (sound->getVolume() != newVolume)
            sound->setVolume(newVolume);
    });

    std::for_each(growlingSounds.begin(), growlingSounds.end(), [=](auto sound) {
        if (sound->getVolume() != newVolume)
            sound->setVolume(newVolume);
    });
}

void Enemy::move(sf::Vector2f& offset)
{
    m_sprite.move(offset);
}

void Enemy::moveTo(sf::Vector2f goal)
{
    float distance = Utils::getDistanceBetweenPoints(getPosition(), goal);
    int enemyCell = Utils::vectorToNode(getPosition());
    int goalCell = Utils::vectorToNode(goal);

    if (distance < 200)
    {
        std::cout << "DEBUG: A small path is being generated..." << std::endl;
        AStarMovement(enemyCell, goalCell);
    }
    else
    {
        std::vector<int> finalPath;
        std::cout << "DEBUG: A long path is being generated..." << std::endl;

        // Get the waypoint closer to the zombie
        int closestWaypoint = m_grid.getClosestWaypoint(getPosition());

        std::vector<int> startToFirstWPPath = calculateAStarPath(enemyCell, closestWaypoint);
        for (auto& node : startToFirstWPPath)
            finalPath.push_back(node);

        // When the waypoint is reached, get the waypoint closer to the goal
        int closestWaypointFromGoal = m_grid.getClosestWaypoint(goal);

        std::cout << "Closest waypoint: " <<  closestWaypoint << std::endl;
        std::cout << "Closest waypoint from goal: " <<  closestWaypointFromGoal << std::endl;

        // Using the two found waypoints, search for one of the pre-computed UCS Path
        auto ucsPath = m_grid.getUCSPath(closestWaypoint, closestWaypointFromGoal);
        if (ucsPath.empty()) // Use only AStar if no UCS path has been found
        {
            AStarMovement(enemyCell, goalCell);
            return;
        }

        finalPath.insert(finalPath.end(), ucsPath.begin(), ucsPath.end()); // Append the UCS path to the final path

        std::vector<int> SecondWPToGoalPath = calculateAStarPath(closestWaypointFromGoal, goalCell);

        for (auto& node : SecondWPToGoalPath)
            finalPath.push_back(node);

        auto last = std::unique(finalPath.begin(), finalPath.end());
        finalPath.erase(last, finalPath.end());

        drawPath(finalPath);

        std::for_each(finalPath.begin(), finalPath.end(), [](int val) { std::cout << val << " "; });
        std::cout << std::endl;

        m_ucsPath = finalPath;
    }

}

void Enemy::changeDebug()
{
    debugActive = !debugActive;
}

void Enemy::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    target.draw(m_sprite);
    if (debugActive)
    {
        target.draw(m_path);
    }
}

/**
 *
 *
 * @param startNodeIdx
 * @param goalNodeIdx
 * @return the list of node in the
 */
std::vector<int> Enemy::calculateAStarPath(int startNodeIdx, int goalNodeIdx)
{
    std::vector<Node*> path;
    m_grid.clearMarks();
    m_grid.aStar(m_grid.nodeIndex(startNodeIdx), m_grid.nodeIndex(goalNodeIdx), path);
    std::reverse(path.begin(), path.end());

    std::vector<int> pathIdx;

    pathIdx.reserve(path.size());
    for (auto& node : path)
        pathIdx.push_back(node->m_data.id);

    return pathIdx;
}

void Enemy::AStarMovement(int startNodeIdx, int goalNodeIdx)
{
    std::vector<int> finalPath;

    finalPath = calculateAStarPath(startNodeIdx, goalNodeIdx);

    std::for_each(finalPath.begin(), finalPath.end(), [](int val) { std::cout << val << " "; });
    std::cout << std::endl;

    drawPath(finalPath);
    m_ucsPath = finalPath;
}






