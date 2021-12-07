#include "Player.h"


void Player::unitVector(sf::Vector2f& t_vector, float dt)
{
    float length = sqrt((t_vector.x * t_vector.x) + (t_vector.y * t_vector.y));
    t_vector = (t_vector * m_speed * dt) /length;
}

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
    std::ifstream spriteSheetData("ASSETS/IMAGES/sprite_sheets/data/characters_sprite_sheet.json");
    nlohmann::json json;
    spriteSheetData >> json;

    for (auto& val : json["frames"])
    {
        std::string filename = val["filename"];

        std::string::size_type idleFound = filename.find("player/idling/");
        std::string::size_type runningFound = filename.find("player/running/");
        std::string::size_type throwingFound = filename.find("player/throwing/");
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

   

    m_powerBar.setSize(m_powerBarSize);
    m_powerBar.setFillColor(sf::Color::White);
    m_powerthrow.setSize(m_powerSize);
    m_powerthrow.setOutlineThickness(1);
    m_powerthrow.setFillColor(sf::Color::Red);

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
    boundryCheck();

    if (m_throw)
    {
        m_powerBar.setPosition(m_sprite.getPosition().x, m_sprite.getPosition().y);
        m_powerBar.setRotation(m_sprite.getRotation());

        if (m_powerSize.x < m_powerBarSize.x)
        {
            m_powerSize.x++;
        }

        m_powerthrow.setSize(m_powerSize);
        m_powerthrow.setPosition(m_sprite.getPosition().x, m_sprite.getPosition().y);
        m_powerthrow.setRotation(m_sprite.getRotation());
    }    
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
        if (event.key.code == sf::Keyboard::RControl)
        {
            m_throw = false;
            m_powerSize.x = 0;
            
        }
    }
}

void Player::awayFrom(sf::Vector2f t_obstacle)
{
    sf::Vector2f gap = t_obstacle - m_sprite.getPosition();
    gap = (getRadius() / sqrt((gap.x * gap.x) + (gap.y * gap.y))) * gap;
    m_sprite.setPosition(t_obstacle - gap);
}

void Player::renderPowerBar(sf::RenderWindow& t_window)
{
    if (m_throw)
    { 
        t_window.draw(m_powerBar);   
        t_window.draw(m_powerthrow);
    }
}

void Player::move(float dt)
{
    m_velocity = sf::Vector2f(0.0f, 0.0f);
    //m_playerState = PlayerMovingState::WALKING;

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::RControl))
    {
        m_throw = true;
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
    {
        m_velocity.y += -m_speed * dt;
        m_playerState = PlayerMovingState::WALKING;
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
    {
        m_playerState = PlayerMovingState::WALKING;
        m_velocity.y += m_speed * dt;
    }

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
    {
        m_playerState = PlayerMovingState::WALKING;
        m_velocity.x += -m_speed * dt;
    }

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
    {
        m_playerState = PlayerMovingState::WALKING;
        m_velocity.x += m_speed * dt;
    }

    if (m_velocity.y > 0 && m_velocity.x > 0)
    {
        setDirection(SOUTHEAST);
        unitVector(m_velocity, dt);
    }
    else if (m_velocity.y > 0 && m_velocity.x < 0)
    {
        setDirection(SOUTHWEST);
        unitVector(m_velocity, dt);
    }
    else if (m_velocity.y < 0 && m_velocity.x > 0)
    {
        setDirection(NORTHEAST);
        unitVector(m_velocity, dt);
    }
    else if (m_velocity.y < 0 && m_velocity.x < 0)
    {
        setDirection(NORTHWEST);
        unitVector(m_velocity, dt);
    }
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

sf::Sprite Player::getSprite()
{
    return m_sprite;
}

void Player::boundryCheck()
{
    if (m_sprite.getPosition().x > screen_Width - m_sprite.getTextureRect().width)
    {
        m_sprite.setPosition(screen_Width - m_sprite.getTextureRect().width, m_sprite.getPosition().y);
    }
    if (m_sprite.getPosition().x < getRadius())
    {
        m_sprite.setPosition(getRadius(), m_sprite.getPosition().y);
    }
    if (m_sprite.getPosition().y > screen_Height - m_sprite.getTextureRect().height)
    {
        m_sprite.setPosition(m_sprite.getPosition().x, screen_Height - m_sprite.getTextureRect().height);
    }
    if (m_sprite.getPosition().y < getRadius())
    {
        m_sprite.setPosition(m_sprite.getPosition().x, getRadius());
    }
}

