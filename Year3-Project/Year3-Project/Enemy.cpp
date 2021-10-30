#include "Enemy.h"

Enemy::Enemy()
{
    m_speed = 2;
    Enemy::loadImage();
    Enemy::setDirection(WEST);

    m_sprite.setOrigin(17.5, 21.5);
    m_sprite.setPosition(300, 300);
    m_sprite.setTextureRect(sf::IntRect(424, 0, 35, 43));

    m_EnemyState = EnemyState::IDLE;

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

void Enemy::update(float dt)
{
    //pointInTriangle(m_visionP1, m_visionP2, m_visionP3, m_playerLocation);
    setVisionCone(30);
}

void Enemy::renderVisionCone(sf::RenderWindow& t_window)
{
     t_window.draw(coneVision);
}

void Enemy::visionConeCollisionCheck(Player& player)
{
}

void Enemy::pointInTriangle(sf::Vector2f t_p1, sf::Vector2f t_p2, sf::Vector2f t_p3, sf::Vector2f t_targetLocation)
{
}

void Enemy::setVisionCone(float t_angle)
{
    coneVision = sf::VertexArray(sf::Triangles, 3);
    m_visionP1 = m_sprite.getPosition();
    coneVision[1].position = m_visionP2;
    coneVision[2].position = m_visionP3;

    if (m_EnemyState == EnemyState::IDLE)
    {
        coneVision[0].position = m_visionP1;

        m_visionP1 = m_sprite.getPosition() + (VISION_CONE_LENGTH_SEEK * rotatedVector(m_visionConeDir, -t_angle + m_sprite.getRotation()));
        m_visionP2 = m_sprite.getPosition() + (VISION_CONE_LENGTH_SEEK * rotatedVector(m_visionConeDir, t_angle + m_sprite.getRotation()));

        

        coneVision[0].color = sf::Color::Green;
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

    float cos = std::cos(t_angle*PI/180);
    float sin = std::sin(t_angle*PI/180);

    // Don't manipulate x and y separately, otherwise they're overwritten too early
    vector = sf::Vector2f(
        cos * vector.x - sin * vector.y,
        sin * vector.x + cos * vector.y);
}



