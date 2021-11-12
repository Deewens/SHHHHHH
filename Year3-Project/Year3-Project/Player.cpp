#include "Player.h"

Player::Player()
{
    m_speed = WALKING_SPEED;
    Player::loadImage();

    m_sprite.setOrigin(19, 22);
    m_sprite.setPosition(100, 100);
    m_sprite.setTextureRect(sf::IntRect(351, 132, 38, 44));
    
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

void Player::update(float dt)
{
    boundryCheck();
    move(dt);
}

void Player::processEvents(sf::Event event)
{
    if (event.type == sf::Event::KeyPressed)
    {
        if (event.key.code == sf::Keyboard::Up)
        {
            m_isMoving.up = true;
        }

        if (event.key.code == sf::Keyboard::Down)
        {
            m_isMoving.down = true;
        }

        if (event.key.code == sf::Keyboard::Left)
        {
            m_isMoving.left = true;
        }
        if (event.key.code == sf::Keyboard::Right)
        {
            m_isMoving.right = true;
        }

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

    if (event.type == sf::Event::KeyReleased)
    {
        if (event.key.code == sf::Keyboard::P)
        {
            m_gameState = GameState::PAUSE;
        }  
        if (event.key.code == sf::Keyboard::Up)
        {
            m_isMoving.up = false;
        }

        if (event.key.code == sf::Keyboard::Down)
        {
            m_isMoving.down = false;
        }

        if (event.key.code == sf::Keyboard::Left)
        {
            m_isMoving.left = false;
        }
        if (event.key.code == sf::Keyboard::Right)
        {
            m_isMoving.right = false;
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

    if (m_isMoving.up)
        m_velocity.y += -m_speed * dt;

    if (m_isMoving.down)
        m_velocity.y += m_speed * dt;

    if (m_isMoving.left)
        m_velocity.x += -m_speed * dt;

    if (m_isMoving.right)
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