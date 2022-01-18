#ifndef YEAR3_PROJECT_NODEDATA_H
#define YEAR3_PROJECT_NODEDATA_H

struct NodeData
{
    int id = 0;
    sf::Vector2f position;
    bool isPassable = true;
    float cost = 0.0f;
};


#endif //YEAR3_PROJECT_NODEDATA_H
