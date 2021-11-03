#ifndef YEAR3_PROJECT_ENEMY_H
#define YEAR3_PROJECT_ENEMY_H

#include "Character.h"
#include<math.h>
#include"Globals.h"
#include"Player.h"
#include"MyVector2.h"

class Enemy : public Character
{
public:
    Enemy();

    // Overrides

    void setDirection(int t_direction);

    void update(float dt) override;

    void renderVisionCone(sf::RenderWindow& t_window);

    void visionConeCollisionCheck(sf::Vector2f t_playerLocation);

    void pointInTriangle(sf::Vector2f t_p1, sf::Vector2f t_p2, sf::Vector2f t_p3);

    void setVisionCone(float t_angel);

    sf::Vector2f rotatedVector(const sf::Vector2f& vector, float t_angle);

    void rotate(sf::Vector2f& vector, float t_angle);

    void move(sf::Vector2f t_startVec, sf::Vector2f t_finishVec);


private:

       
    sf::VertexArray coneVision;

    const float VISION_CONE_LENGTH_SEEK{ 200.0f };
    const float VISION_CONE_LENGTH_ATTACK{ 300.0f };

    sf::Vector2f m_visionConeDir{1, 0};

    sf::Vector2f m_visionConeLeft;

    sf::Vector2f m_visionConeRight;

    sf::Vector2f m_visionP1{};
    sf::Vector2f m_visionP2{};
    sf::Vector2f m_visionP3{};

    sf::Vector2f m_playerLocation{};

    sf::Vector2f m_directionStart{};
    sf::Vector2f m_directionEnd{};

    sf::Vertex m_directionLine0;
    sf::Vertex m_directionLine1;

    sf::Vector2f m_movementVector{};

    enum class EnemyState
    {
        SEEK,
        ATTACK
    }m_EnemyState;

};


#endif //YEAR3_PROJECT_ENEMY_H
