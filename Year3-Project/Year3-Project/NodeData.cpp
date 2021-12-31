#include "NodeData.h"

NodeData::NodeData(int id, bool isPassable = true) : m_id(id), m_isPassable(isPassable)
{
}

int NodeData::getId() const
{
    return m_id;
}

void NodeData::setPassable(bool passable)
{
    m_isPassable = passable;
}

bool NodeData::isPassable() const
{
    return m_isPassable;
}


