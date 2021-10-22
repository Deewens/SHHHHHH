#ifndef YEAR3_PROJECT_ENEMY_H
#define YEAR3_PROJECT_ENEMY_H

#include "Character.h"

class Enemy : public Character
{
public:
    Enemy();

    // Overrides

    void setDirection(int t_direction);

    void update() override;
};


#endif //YEAR3_PROJECT_ENEMY_H
