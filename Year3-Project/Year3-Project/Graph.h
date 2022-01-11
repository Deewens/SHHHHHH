#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <set>
#include <queue>
#include <functional>

#include <SFML/Graphics.hpp>

#include "Globals.h"
#include "GraphArc.h"
#include "GraphNode.h"
#include "NodeData.h"
#include "NodeComparer.h"

template<typename NodeType, class ArcType>
class Graph : public sf::Drawable
{
public:
    // typedef the classes to make our lives easier.
    typedef GraphArc<NodeType, ArcType> Arc;
    typedef GraphNode<NodeType, ArcType> Node;

    Graph(int maxNodes);

    Graph(int t_rows, int t_cols, int size);

    ~Graph();

    void render(sf::RenderWindow &t_window);

    void toggleDraw();

    void debug();

    std::vector<std::vector<Node>> getGrid();

    // Accessors
    Node *nodeIndex(int index) const
    {
        return m_nodes.at(index);
    }

    // Public member functions.
    bool addNode(NodeType data, int index);

    void removeNode(int index);

    bool addArc(int from, int to, ArcType weight);

    void removeArc(int from, int to);

    Arc *getArc(int from, int to);

    void clearMarks();

    std::map<int, float> getNeighbours(int node);

    void ucs(Node *start, Node *goal, std::function<void(Node *)> f_visit, std::vector<Node *> &path);

private:
    int m_rows;
    int m_cols;

    std::vector<Node*> m_nodes;

    sf::Vertex m_colLine[96];
    sf::Vertex m_rowLine[60];

    bool m_gridDraw = false;

    void draw(sf::RenderTarget &target, sf::RenderStates states) const final;


    int m_size = 0;

    sf::Font m_gridFont;

    sf::Text m_gridTextElement;
    std::vector<sf::Text> m_gridText;
};

template<typename NodeType, typename ArcType>
Graph<NodeType, ArcType>::Graph(int maxNodes) : m_rows(0), m_cols(0), m_nodes(maxNodes, nullptr)
{
}

template<typename NodeType, typename ArcType>
Graph<NodeType, ArcType>::Graph(int t_rows, int t_cols, int maxNodes) : m_rows(t_rows), m_cols(t_cols), m_nodes(maxNodes, nullptr)
{
    m_size = t_rows * t_cols;

    for (int i = 0; i < m_cols; i++)
    {
        m_colLine[i * 2] = sf::Vertex(sf::Vector2f{(float) (i * tileSize), 0});
        m_colLine[i * 2 + 1] = sf::Vertex(sf::Vector2f{(float) (i * tileSize), screen_Height});
    }

    for (int i = 0; i < m_rows; i++)
    {
        m_rowLine[i * 2] = sf::Vertex(sf::Vector2f{0, (float) (i * tileSize)});
        m_rowLine[i * 2 + 1] = sf::Vertex(sf::Vector2f{screen_Width, (float) (i * tileSize)});
    }

    int id = 0;

    for (int i = 0; i < m_rows; i++)
    {
        for (int j = 0; j < m_cols; j++)
        {
            NodeData nodeData = NodeData{id};
            addNode(nodeData, id++);
        }
    }

/*    for (int i = 0; i < m_nodes.size(); i++)
    {
        std::map<int, float> neighbours = getNeighbours(i);
        for (auto neighbour : neighbours)
        {
            addArc(i, neighbour.first, neighbour.second);
        }
    }*/

    std::cout << "Grid initialization" << std::endl;
    auto neighbours = getNeighbours(5);
    for (auto& neighbour : neighbours)
    {
        std::cout << "NODE: " << neighbour.first << " - COST: " << neighbour.second << std::endl;
    }
}

// ----------------------------------------------------------------
//  Name:           ~Graph
//  Description:    destructor, This deletes every node
//  Arguments:      None.
//  Return Value:   None.
// ----------------------------------------------------------------
template<typename NodeType, typename ArcType>
Graph<NodeType, ArcType>::~Graph()
{
    for (int index = 0; index < m_nodes.size(); index++)
    {
        if (m_nodes[index] != nullptr)
        {
            delete m_nodes.at(index);
        }
    }
}

template<typename NodeType, typename ArcType>
void Graph<NodeType, ArcType>::toggleDraw()
{
    m_gridDraw = !m_gridDraw;
}

template<typename NodeType, typename ArcType>
void Graph<NodeType, ArcType>::draw(sf::RenderTarget &target, sf::RenderStates states) const
{
    if (m_gridDraw)
    {
        target.draw(m_colLine, 96, sf::Lines);
        target.draw(m_rowLine, 60, sf::Lines);
        for (int i = 0; i < m_size; i++)
        {
            target.draw(m_gridText[i]);
        }
    }
}

template<typename NodeType, typename ArcType>
void Graph<NodeType, ArcType>::debug()
{
    if (!m_gridFont.loadFromFile("ASSETS\\FONTS\\ariblk.ttf"))
    {
        std::cout << "Problem loading Grid Font" << std::endl;
    }
    m_gridTextElement.setFont(m_gridFont);
    m_gridTextElement.setCharacterSize(20);
    m_gridTextElement.setFillColor(sf::Color::Black);

    int m_id = 0;

    for (int i = 0; i < m_rows; i++)
    {
        for (int j = 0; j < m_cols; j++)
        {
            m_gridTextElement.setString(std::to_string(m_id));
            m_gridTextElement.setPosition(j * tileSize, i * tileSize);
            m_gridText.push_back(m_gridTextElement);
            m_id++;
        }
    }
}

template<typename NodeType, typename ArcType>
std::vector<std::vector<GraphNode<NodeType, ArcType>>> Graph<NodeType, ArcType>::getGrid()
{
    std::vector<std::vector<Node>> grid;
    grid.reserve(m_rows * m_cols);

    int index = 0;

    for (int i = 0; i < m_rows; i++)
    {
        std::vector<GraphNode<NodeType, ArcType>> row;

        for (int j = 0; j < m_cols; j++)
        {
            row.push_back(*m_nodes.at(index));
            index++;
        }
        grid.push_back(row);
    }

    return grid;
}

// ----------------------------------------------------------------
//  Name:           addNode
//  Description:    This adds a node at a given index in the graph.
//  Arguments:      The first parameter is the data to store in the node.
//                  The second parameter is the index to store the node.
//  Return Value:   true if successful
// ----------------------------------------------------------------
template<class NodeType, class ArcType>
bool Graph<NodeType, ArcType>::addNode(NodeType data, int index)
{
    bool nodeNotPresent = false;
    // find out if a node does not exist at that index.
    if (nullptr == m_nodes.at(index))
    {
        nodeNotPresent = true;
        // create a new node, put the data in it, and unmark it.
        m_nodes.at(index) = new Node;
        m_nodes.at(index)->m_data = data;
        m_nodes.at(index)->setMarked(false);
    }

    return nodeNotPresent;
}

// ----------------------------------------------------------------
//  Name:           removeNode
//  Description:    This removes a node from the graph
//  Arguments:      The index of the node to return.
//  Return Value:   None.
// ----------------------------------------------------------------
template<class NodeType, class ArcType>
void Graph<NodeType, ArcType>::removeNode(int index)
{
    // Only proceed if node does exist.
    if (nullptr != m_nodes.at(index))
    {
        // now find every arc that points to the node that
        // is being removed and remove it.
        Arc *arc;

        // loop through every node
        for (int node = 0; node < m_nodes.size(); node++)
        {
            // if the node is valid...
            if (nullptr != m_nodes.at(node))
            {
                // see if the node has an arc pointing to the current node.
                arc = m_nodes.at(node)->getArc(m_nodes.at(index));
            }
            // if it has an arc pointing to the current node, then
            // remove the arc.
            if (arc != 0)
            {
                removeArc(node, index);
            }
        }


        // now that every arc pointing to the current node has been removed,
        // the node can be deleted.
        delete m_nodes.at(index);
        m_nodes.at(index) = nullptr;
    }
}

// ----------------------------------------------------------------
//  Name:           addArd
//  Description:    Adds an arc from the first index to the
//                  second index with the specified weight.
//  Arguments:      The first argument is the originating node index
//                  The second argument is the ending node index
//                  The third argument is the weight of the arc
//  Return Value:   true on success.
// ----------------------------------------------------------------
template<class NodeType, class ArcType>
bool Graph<NodeType, ArcType>::addArc(int from, int to, ArcType weight)
{
    bool proceed = true;
    // make sure both nodes exist.
    if (nullptr == m_nodes.at(from) || nullptr == m_nodes.at(to))
    {
        proceed = false;
    }

    // if an arc already exists we should not proceed
    if (m_nodes.at(from)->getArc(m_nodes.at(to)) != nullptr)
    {
        proceed = false;
    }

    if (proceed == true)
    {
        // add the arc to the "from" node.
        m_nodes.at(from)->addArc(m_nodes.at(to), weight);
    }

    return proceed;
}

// ----------------------------------------------------------------
//  Name:           removeArc
//  Description:    This removes the arc from the first index to the second index
//  Arguments:      The first parameter is the originating node index.
//                  The second parameter is the ending node index.
//  Return Value:   None.
// ----------------------------------------------------------------
template<class NodeType, class ArcType>
void Graph<NodeType, ArcType>::removeArc(int from, int to)
{
    // Make sure that the node exists before trying to remove
    // an arc from it.
    bool nodeExists = true;
    if (nullptr == m_nodes.at(from) || nullptr == m_nodes.at(to))
    {
        nodeExists = false;
    }

    if (nodeExists == true)
    {
        // remove the arc.
        m_nodes.at(from)->removeArc(m_nodes.at(to));
    }
}


// ----------------------------------------------------------------
//  Name:           getArc
//  Description:    Gets a pointer to an arc from the first index
//                  to the second index.
//  Arguments:      The first parameter is the originating node index.
//                  The second parameter is the ending node index.
//  Return Value:   pointer to the arc, or 0 if it doesn't exist.
// ----------------------------------------------------------------
template<class NodeType, class ArcType>
GraphArc<NodeType, ArcType> *Graph<NodeType, ArcType>::getArc(int from, int to)
{
    Arc *arc = 0;
    // make sure the to and from nodes exist
    if (nullptr != m_nodes.at(from) && nullptr != m_nodes.at(to))
    {
        arc = m_nodes.at(from)->getArc(m_nodes.at(to));
    }

    return arc;
}


// ----------------------------------------------------------------
//  Name:           clearMarks
//  Description:    This clears every mark on every node.
//  Arguments:      None.
//  Return Value:   None.
// ----------------------------------------------------------------
template<class NodeType, class ArcType>
void Graph<NodeType, ArcType>::clearMarks()
{
    for (int index = 0; index < m_nodes.size(); index++)
    {
        if (nullptr != m_nodes.at(index))
        {
            m_nodes.at(index)->setMarked(false);
        }
    }
}

/*
 * start is a Graph node indicating the origin of the search
 * dest is the goal vertex indicating the destination of the search
 * f_visit is a function that outputs the node currently being expanded (i.e. the node at the top of the priority queue).
 * path is a container (a vector) which holds the best path generated by the algorithm’s completion
 */
template<class NodeType, class ArcType>
void Graph<NodeType, ArcType>::ucs(Graph::Node *start, Graph::Node *goal, std::function<void(Node *)> f_visit,
                                   std::vector<Node *> &path)
{
    if (start == nullptr || goal == nullptr) return;

    std::priority_queue<Node *, std::vector<Node *>, NodeComparer< NodeType, ArcType>>
    pq;

    for (int i = 0; i < m_nodes.size(); i++)
    {
        m_nodes.at(i)->m_data.m_cost = -1;
    }

    start->m_data.m_cost = 0;

    pq.push(start);
    start->setMarked(true);

    while (!pq.empty() && pq.top() != goal)
    {
        auto it = pq.top()->arcList().begin();
        auto endIt = pq.top()->arcList().end();

        for (; it != endIt; it++)
        {
            //Arc *arc = it;
            Node *child = it->node();
            if (child != pq.top()->previous())
            {
                int arcWeight = it->weight();
                int distToChild = arcWeight + pq.top()->m_data.m_cost;
                //auto distToChild = pq.top()->m_data.m_cost + (*it).node()->m_data.m_cost + pq.;

                if (distToChild < child->m_data.m_cost || child->m_data.m_cost == -1)
                {
                    (*it).node()->m_data.m_cost = distToChild;
                    (*it).node()->setPrevious(pq.top());
                }

                if (!child->marked())
                {
                    pq.push(child);
                    child->setMarked(true);
                }
            }
        }
        pq.pop();
    }

    Node *temp = goal;

    for (; temp != nullptr; temp = temp->previous())
    {
        path.push_back(temp);
    }
}

/**
* Get the neighbours of the given node
*
* @param grid get the neighbours of the node from this grid
* @param node the node from which to get the neighbours
* @return the list of node, neighbours of the given node
*/
template<typename NodeType, class ArcType>
std::map<int, float> Graph<NodeType, ArcType>::getNeighbours(int node)
{
    // Get the row and col index of the node

    auto vectorGrid = getGrid();

    int row = 0;
    int col = 0;
    for (int i = 0; i < vectorGrid.size(); i++)
    {
        for (int j = 0; j < vectorGrid[i].size(); j++)
        {
            if (vectorGrid[i][j].m_data.id == node)
            {
                row = i;
                col = j;
                break;
            }
        }
    }

    // Get the neighbours of the node
    std::map<int, float> neighbours;

    // List all neighbors:
    for (int direction = 0; direction < 9; direction++)
    {
        if (direction == 4) continue; // Skip 4, this is ourself.

        float cost = 1;
        if (direction == 0 || direction == 2 || direction == 6 || direction == 8) // Diagonal
            cost = 1.5f;

        int n_row = row + ((direction % 3) - 1); // Neighbor row
        int n_col = col + ((direction / 3) - 1); // Neighbor column

        // Check the bounds:
        if (n_row >= 0 && n_row < vectorGrid.size() && n_col >= 0 && n_col < vectorGrid[n_row].size())
        {
            // A valid neighbor:
            if (vectorGrid[n_row][n_col].m_data.isPassable) neighbours.insert({vectorGrid[n_row][n_col].m_data.id, cost});
        }
    }

    return neighbours;
}

