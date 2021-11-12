#include "Player.h"

Player::Player() :
    m_runningAnim(m_sprite, true),
    m_walkingAnim(m_sprite, true),
    m_crouchingAnim(m_sprite, true),
    m_idlingAnim(m_sprite, true),
    m_throwingAnim(m_sprite, true)
{
    m_speed = WALKING_SPEED;
    Player::loadTexture();

    // Get spritesheet data
    std::ifstream spriteSheetData("ASSETS/IMAGES/Player/PlayerSpriteSheet.json");
    nlohmann::json json;
    spriteSheetData >> json;

    for (auto& val : json["frames"])
    {
        std::string filename = val["filename"];

        std::string::size_type idleFound = filename.find("Idle");
        std::string::size_type runningFound = filename.find("Running");
        std::string::size_type throwingFound = filename.find("Throwing");
        // Found something
        if (idleFound != std::string::npos)
        {
            nlohmann::json frame = val["frame"];
            int x = frame["x"];
            int y = frame["y"];
            int width = frame["w"];
            int height = frame["h"];

            m_idlingAnim.addFrame({sf::IntRect(x, y, width, height)});
        }
        else if (runningFound != std::string::npos)
        {
            nlohmann::json frame = val["frame"];
            int x = frame["x"];
            int y = frame["y"];
            int width = frame["w"];
            int height = frame["h"];

            m_runningAnim.addFrame({sf::IntRect(x, y, width, height), 0.05});
            m_walkingAnim.addFrame({sf::IntRect(x, y, width, height), 0.2});
            m_crouchingAnim.addFrame({sf::IntRect(x, y, width, height), 0.5});
        }
        else if (throwingFound != std::string::npos)
        {
            nlohmann::json frame = val["frame"];
            int x = frame["x"];
            int y = frame["y"];
            int width = frame["w"];
            int height = frame["h"];

            m_throwingAnim.addFrame({sf::IntRect(x, y, width, height)});
        }
    }

    m_sprite.setPosition(100, 100);
    m_sprite.setOrigin(30, 26);
}



void Player::setDirection(int t_direction)
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

void Player::update(sf::Time deltaTime)
{
    move(deltaTime.asSeconds());
    if (m_velocity.x == 0 && m_velocity.y == 0)
        m_playerState = PlayerMovingState::IDLE;

    // Update animations
    switch (m_playerState)
    {
        case PlayerMovingState::RUNNING:
            m_runningAnim.update(deltaTime.asSeconds());
            break;
        case PlayerMovingState::WALKING:
            m_walkingAnim.update(deltaTime.asSeconds());
            break;
        case PlayerMovingState::CROUCHING:
            m_crouchingAnim.update(deltaTime.asSeconds());
            break;
        case PlayerMovingState::IDLE:
            m_idlingAnim.update(deltaTime.asSeconds());
            break;
        default:
            m_idlingAnim.update(deltaTime.asSeconds());
    }

    //std::cout << m_velocity.x << ", " << m_velocity.y << std::endl;

    boundryCheck();
}

void Player::processEvents(sf::Event event)
{
    if (event.type == sf::Event::KeyPressed)
    {
        if (event.key.code == sf::Keyboard::LControl || event.key.code == sf::Keyboard::Space)
        {
            if (event.key.code == sf::Keyboard::LControl)
            {
                m_speed = CROUCHING_SPEED;
                m_playerState = PlayerMovingState::CROUCHING;
            }
            if (event.key.code == sf::Keyboard::Space)
            {
                m_speed = RUNNING_SPEED;
                m_playerState = PlayerMovingState::RUNNING;
            }
        }
    }

    if (event.type == sf::Event::KeyReleased)
    {
        if (event.key.code == sf::Keyboard::P)
        {
            m_gameState = GameState::PAUSE;
        }

        if (event.key.code == sf::Keyboard::LControl || event.key.code == sf::Keyboard::Space)
        {
            m_speed = WALKING_SPEED;
            m_playerState = PlayerMovingState::WALKING;
        }
    }
}

void Player::awayFrom(sf::Vector2f t_obstacle)
{
    sf::Vector2f gap = t_obstacle - m_sprite.getPosition();
    gap = (getRadius() / sqrt((gap.x * gap.x) + (gap.y * gap.y))) * gap;
    m_sprite.setPosition(t_obstacle - gap);
}

void Player::move(float dt)
{
    m_velocity = sf::Vector2f(0.0f, 0.0f);
    //m_playerState = PlayerMovingState::WALKING;

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
        m_velocity.y += -m_speed * dt;

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
        m_velocity.y += m_speed * dt;

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
        m_velocity.x += -m_speed * dt;

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
        m_velocity.x += m_speed * dt;

    if (m_velocity.y > 0 && m_velocity.x > 0)
        setDirection(SOUTHEAST);
    else if (m_velocity.y > 0 && m_velocity.x < 0)
        setDirection(SOUTHWEST);
    else if (m_velocity.y < 0 && m_velocity.x > 0)
        setDirection(NORTHEAST);
    else if (m_velocity.y < 0 && m_velocity.x < 0)
        setDirection(NORTHWEST);
    else if (m_velocity.y > 0)
        setDirection(SOUTH);
    else if (m_velocity.y < 0)
        setDirection(NORTH);
    else if (m_velocity.x > 0)
        setDirection(EAST);
    else if (m_velocity.x < 0)
        setDirection(WEST);

    m_sprite.move(m_velocity);
}

sf::Vector2f Player::getVelocity()
{
    return m_velocity;
}

void Player::boundryCheck()
{
    if (m_sprite.getPosition().x > screen_Width - m_sprite.getTextureRect().width)
    {
        m_sprite.setPosition(screen_Width - m_sprite.getTextureRect().width, m_sprite.getPosition().y);
    }
    if (m_sprite.getPosition().x < m_sprite.getTextureRect().width)
    {
        m_sprite.setPosition(m_sprite.getTextureRect().width, m_sprite.getPosition().y);
    }
    if (m_sprite.getPosition().y > screen_Height - m_sprite.getTextureRect().height)
    {
        m_sprite.setPosition(m_sprite.getPosition().x, screen_Height - m_sprite.getTextureRect().height);
    }
    if (m_sprite.getPosition().y < m_sprite.getTextureRect().height)
    {
        m_sprite.setPosition(m_sprite.getPosition().x, m_sprite.getTextureRect().height);
    }
}

void Player::loadTexture()
{
    if (!m_texture.loadFromFile("ASSETS/IMAGES/Player/PlayerSpriteSheet.png"))
        std::cout << "problem loading character texture" << std::endl;

    m_sprite.setTexture(m_texture);
}