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
            m_playerState = PlayeMovingState::CROUCHING;
        }

        if (event.key.code == sf::Keyboard::Space)
        {
            m_speed = RUNNING_SPEED;
            m_playerState = PlayeMovingState::RUNNING;
        }
    }

    if (event.type == sf::Event::KeyReleased)
    {
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
            m_playerState = PlayeMovingState::WALKING;
        }
    }
}

void Player::move(float dt)
{
    sf::Vector2f velocity = sf::Vector2f(0, 0);
    velocity.x = 0.f;
    velocity.y = 0.f;

    if (m_isMoving.up)
        velocity.y += -m_speed * dt;

    if (m_isMoving.down)
        velocity.y += m_speed * dt;

    if (m_isMoving.left)
        velocity.x += -m_speed * dt;

    if (m_isMoving.right)
        velocity.x += m_speed * dt;

    if (velocity.y > 0 && velocity.x > 0)
        setDirection(SOUTHEAST);
    else if (velocity.y > 0 && velocity.x < 0)
        setDirection(SOUTHWEST);
    else if (velocity.y < 0 && velocity.x > 0)
        setDirection(NORTHEAST);
    else if (velocity.y < 0 && velocity.x < 0)
        setDirection(NORTHWEST);
    else if (velocity.y > 0)
        setDirection(SOUTH);
    else if (velocity.y < 0)
        setDirection(NORTH);
    else if (velocity.x > 0)
        setDirection(EAST);
    else if (velocity.x < 0)
        setDirection(WEST);

    m_sprite.move(velocity);

    // Prevent player from going outside the screen
    if (m_sprite.getPosition().x < 0.f)
        m_sprite.setPosition(0.f, m_sprite.getPosition().y);

    if (m_sprite.getPosition().y < 0.f)
        m_sprite.setPosition(m_sprite.getPosition().x, 0.f);

    if (m_sprite.getPosition().x + m_sprite.getGlobalBounds().width > screen_Width)
        m_sprite.setPosition(screen_Width - m_sprite.getGlobalBounds().width, m_sprite.getPosition().y);

    if (m_sprite.getPosition().y + m_sprite.getGlobalBounds().height > screem_Height)
        m_sprite.setPosition(m_sprite.getPosition().x, screem_Height - m_sprite.getGlobalBounds().height);

}
