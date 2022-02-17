#include "Enemy.h"

Enemy::Enemy() :
        m_runningAnim(m_sprite),
        m_idlingAnim(m_sprite)
{
    Enemy::loadTexture();
    Enemy::setDirection(EAST);

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

            m_idlingAnim.addFrame({sf::IntRect(x, y, width, height)});
        }
        else if (runningFound != std::string::npos)
        {
            nlohmann::json frame = frames["frame"];
            int x = frame["x"];
            int y = frame["y"];
            int width = frame["w"];
            int height = frame["h"];

            m_runningAnim.addFrame({sf::IntRect(x, y, width, height), 0.05});
        }
    }

    m_sprite.setOrigin(30, 30);
    m_sprite.setPosition(300, 300);

    m_EnemyState = EnemyState::SEEK;

    m_searchCounter = 500;
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

void Enemy::update(sf::Time deltaTime , Graph<NodeData, float> t_grid)
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
    //pathFinding(t_grid);
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

void Enemy::pathFinding(Graph<NodeData, float> t_grid )
{
    int m_secondToLastCell = 0;
    int m_lastCell = 0;

    int playerCell = floor(m_playerLocation.x / tileSize) + (floor(m_playerLocation.y / tileSize) * screen_Width / tileSize);
    int zombieCell = floor(m_sprite.getPosition().x / tileSize) + (floor(m_sprite.getPosition().y / tileSize) * screen_Width / tileSize);

    m_searchCounter++;
    if (m_searchCounter >= 50)
    {
        m_searchCounter = 0;
        t_grid.aStar(t_grid.nodeIndex(zombieCell), t_grid.nodeIndex(playerCell),m_path);
    }
    if (m_path.size() > 1)
    {
        m_secondToLastCell = m_path[m_path.size() - 2]->m_data.id;
        if (m_secondToLastCell == zombieCell && m_path.size() > 2)
        {
            m_path.erase(m_path.begin() + m_path.size() - 1);
            m_secondToLastCell = m_path[m_path.size() - 2]->m_data.id;
        }
        m_lastCell = m_path[m_path.size() - 1]->m_data.id;
    }

    int m_columS = m_secondToLastCell % (screen_Width / tileSize);
    int m_rowS = (m_secondToLastCell - m_columS) / (screen_Width / tileSize);

    int m_columF = m_lastCell % (screen_Width / tileSize);
    int m_rowF = (m_lastCell - m_columF) / (screen_Width / tileSize);

    sf::Vector2f m_centerS;
    m_centerS.y = (tileSize / 2) + (tileSize * m_rowS);
    m_centerS.x = (tileSize / 2) + (tileSize * m_columS);

    sf::Vector2f m_centerF;
    m_centerF.y = (tileSize / 2) + (tileSize * m_rowF);
    m_centerF.x = (tileSize / 2) + (tileSize * m_columF);

    move(m_centerS, m_centerF);
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

    for (auto sound : footstepRunSounds)
        sound->setVolume(20);
}

void Enemy::changeSoundsVolume(float newVolume)
{
    for (auto sound : footstepRunSounds)
        if (sound->getVolume() != newVolume)
            sound->setVolume(newVolume);
}

void Enemy::move(sf::Vector2f& offset)
{
    m_sprite.move(offset);
}






