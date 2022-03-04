#ifndef YEAR3_PROJECT_ENEMY_H
#define YEAR3_PROJECT_ENEMY_H

#define _USE_MATH_DEFINES

#include <math.h>
#include <stdlib.h>

#include "Character.h"
#include "Globals.h"
#include "Player.h"

#include "tinyc2.h"
#include "Animation.h"
#include "Utils.h"

typedef GraphNode<NodeData, float> Node;

class Enemy : public Character
{
public:
    Enemy(int t_gridIndex, int rotation, Graph<NodeData, float>& t_grid, SoundHolder& soundHolder);

    void update(sf::Time deltaTime);

    void renderVisionCone(sf::RenderWindow& t_window);

    void visionConeCollisionCheck(sf::Vector2f t_playerLocation);

    void changeSoundsVolume(float newVolume);

    void move(sf::Vector2f& offset);

    void move(sf::Vector2f t_startVec, sf::Vector2f t_finishVec);

    void setPath(int from, int to);

    void moveTo(sf::Vector2f goal);

    void changeDebug();
    
private:

    void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
    
    void setDirection(int t_direction);   

    void setVisionCone(float t_angel);

    sf::Vector2f rotatedVector(const sf::Vector2f& vector, float t_angle);

    void rotate(sf::Vector2f& vector, float t_angle);

    bool isBeingSeen();

    void unitVector(sf::Vector2f& t_vector);

    void debug();

    void pathFinding();
    
    void drawPath(std::vector<int>);
    
    void loadSoundHolder(SoundHolder& soundHolder);

private:

    sf::Texture m_texture;
    sf::VertexArray coneVision;

    Animation m_runningAnim;
    Animation m_idlingAnim;

    std::vector<sf::Sound*> footstepRunSounds;

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

    const float ENEMY_SPEED = 1.0f;

    c2Circle circle_player;

    c2Poly poly_visionCone;

    sf::CircleShape m_playerCircle;

    enum class EnemyState
    {
        SEEK,
        ATTACK
    }m_EnemyState;

    sf::Clock clock;

    int m_searchCounter;

    bool m_isMoving = false;
    sf::Vector2f m_moveTo;

    Graph<NodeData, float>& m_grid;
    std::vector<int> m_ucsPath;

    bool debugActive = false;
    sf::VertexArray m_path;

    std::vector<int> calculateAStarPath(int startNodeIdx, int goalNodeIdx);

    void AStarMovement(int startNodeIdx, int goalNodeIdx);
};


#endif //YEAR3_PROJECT_ENEMY_H
