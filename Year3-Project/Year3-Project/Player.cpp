#include "Player.h"

Player::Player()
{
    m_speed = WALKING_SPEED;
    Player::loadTextures();

    m_idlingAnimation.setSpriteSheet(m_idlingTexture);
    //m_idlingAnimation.addFrame(sf::IntRect(0, 0, 60, 51));
    m_idlingAnimation.addFrame(sf::IntRect(0, 51, 60, 52));
    m_idlingAnimation.addFrame(sf::IntRect(0, 103, 60, 52));
    m_idlingAnimation.addFrame(sf::IntRect(0, 155, 60, 52));
    m_idlingAnimation.addFrame(sf::IntRect(0, 207, 60, 52));
    m_idlingAnimation.addFrame(sf::IntRect(0, 259, 60, 52));
    m_idlingAnimation.addFrame(sf::IntRect(0, 311, 60, 52));
    m_idlingAnimation.addFrame(sf::IntRect(0, 363, 60, 52));
    m_idlingAnimation.addFrame(sf::IntRect(0, 415, 60, 52));
    m_idlingAnimation.addFrame(sf::IntRect(0, 467, 60, 52));
    m_idlingAnimation.addFrame(sf::IntRect(0, 519, 60, 52));
    m_idlingAnimation.addFrame(sf::IntRect(0, 571, 60, 52));
    m_idlingAnimation.addFrame(sf::IntRect(0, 623, 60, 52));
    m_idlingAnimation.addFrame(sf::IntRect(0, 675, 60, 52));
    m_idlingAnimation.addFrame(sf::IntRect(0, 727, 60, 52));
    m_idlingAnimation.addFrame(sf::IntRect(0, 779, 60, 52));
    m_idlingAnimation.addFrame(sf::IntRect(0, 831, 60, 52));
    m_idlingAnimation.addFrame(sf::IntRect(0, 883, 60, 52));
    m_idlingAnimation.addFrame(sf::IntRect(0, 935, 60, 52));
    m_idlingAnimation.addFrame(sf::IntRect(0, 987, 60, 52));

    m_currentAnimation = &m_idlingAnimation;


    m_animatedSprite = AnimatedSprite(sf::seconds(0.2), false, true);
    m_animatedSprite.setPosition(100, 100);
    m_animatedSprite.setOrigin(30, 26);
}



void Player::setDirection(int t_direction)
{
    switch (t_direction)
    {
    case NORTH:
        m_animatedSprite.setRotation(270);
        break;
    case SOUTH:
        m_animatedSprite.setRotation(90);
        break;
    case EAST:
        m_animatedSprite.setRotation(0);
        break;
    case WEST:
        m_animatedSprite.setRotation(180);
        break;
    case NORTHWEST:
        m_animatedSprite.setRotation(225);
        break;
    case NORTHEAST:
        m_animatedSprite.setRotation(315);
        break;
    case SOUTHWEST:
        m_animatedSprite.setRotation(135);
        break;
    case SOUTHEAST:
        m_animatedSprite.setRotation(45);
        break;
    }
}

void Player::update(sf::Time deltaTime)
{
    move(deltaTime.asSeconds());

    m_animatedSprite.update(deltaTime);
    m_animatedSprite.play(*m_currentAnimation);
    boundryCheck();
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
    sf::Vector2f gap = t_obstacle - m_animatedSprite.getPosition();
    gap = (getRadius() / sqrt((gap.x * gap.x) + (gap.y * gap.y))) * gap;
    m_animatedSprite.setPosition(t_obstacle - gap);
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

    m_animatedSprite.move(m_velocity);
}

sf::Vector2f Player::getVelocity()
{
    return m_velocity;
}

void Player::draw(sf::RenderTarget &target, sf::RenderStates states) const
{
    target.draw(m_animatedSprite);
}

void Player::loadTextures()
{
    // Load a default sprite if loadImage is not overrided
    if (!m_idlingTexture.loadFromFile("ASSETS/IMAGES/Player/IdleSpriteSheet.png"))
        std::cout << "problem loading character texture" << std::endl;
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