#pragma once

#include "GraphNode.h"

template <class NodeType, class ArcType>
class NodeComparer
{
public:
    typedef GraphNode<NodeType, ArcType> Node;

    bool operator()(Node const* t_n1, Node const* t_n2) const
    {
        return t_n1->m_data.cost > t_n2->m_data.cost;
    }
};

