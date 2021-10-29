#ifndef YEAR3_PROJECT_ENEMY_H
#define YEAR3_PROJECT_ENEMY_H

#include "Character.h"
#include<math.h>

class Enemy : public Character
{
public:
    Enemy();

    // Overrides

    void setDirection(int t_direction);

    void update(float dt) override;

    void visionConeInit(float t_angle);

    void renderVisionCone(sf::RenderWindow& t_window);

private:

    sf::Vertex coneVision[3] =
    {        
    };
};


#endif //YEAR3_PROJECT_ENEMY_H
