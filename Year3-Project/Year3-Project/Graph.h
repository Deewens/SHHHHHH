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
#include "Utils.h"

template<typename NodeType, class ArcType>
class Graph : public sf::Drawable
{
public:
    // typedef the classes to make our lives easier.
    typedef GraphArc<NodeType, ArcType> Arc;
    typedef GraphNode<NodeType, ArcType> Node;

    explicit Graph(int maxNodes);

    Graph(int t_rows, int t_cols, int maxNodes);

    ~Graph() override;

    void render(sf::RenderWindow& t_window);

    void toggleDraw();

    void debug(sf::Font& t_font);

    std::vector<std::vector<Node>> getGrid();

    // Accessors
    Node* nodeIndex(int index) const
    {
        return m_nodes.at(index);
    }

    // Public member functions.
    bool addNode(NodeType data, int index);

    bool updateNode(NodeData nodeData, int nodeId);

    std::vector<Node*> getNodes();

    void removeNode(int index);

    bool addArc(int from, int to, ArcType weight);

    void removeArc(int from, int to);

    Arc* getArc(int from, int to);

    void clearMarks();

    std::map<int, float> getNeighbours(int node);

    std::vector<int> getWaypoints();
    std::map<std::string, std::vector<int>> getUCSPaths();
    std::vector<int> getUCSPath(int from, int to);

    int getClosestWaypoint(sf::Vector2f pos);

    void aStar(Node* start, Node* dest, std::vector<Node*>& path);

private:
    int m_rows;
    int m_cols;

    std::vector<Node*> m_nodes;

    sf::Vertex m_colLine[96];
    sf::Vertex m_rowLine[60];

    bool m_gridDraw = false;

    void draw(sf::RenderTarget& target, sf::RenderStates states) const final;

    int m_size = 0;

    sf::Font m_gridFont;

    sf::Text m_gridTextElement;
    std::vector<sf::Text> m_gridText;

    std::vector<int> m_waypoints;
    std::map<std::string, std::vector<int>> m_ucsPaths;

    std::vector<sf::RectangleShape> m_pathDraw;
};

template<class NodeType, class ArcType>
class Comparer
{
public:

    typedef GraphArc<NodeType, ArcType> Arc;
    typedef GraphNode<NodeType, ArcType> Node;

    bool operator()(Node* n1, Node* n2)
    {
        return static_cast<int>(n1->m_data.g + n1->m_data.h) > static_cast<int>(n2->m_data.g + n2->m_data.h);
    }
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
        m_colLine[i * 2] = sf::Vertex(sf::Vector2f{ (float)(i * tileSize), 0 });
        m_colLine[i * 2 + 1] = sf::Vertex(sf::Vector2f{ (float)(i * tileSize), screen_Height });
    }

    for (int i = 0; i < m_rows; i++)
    {
        m_rowLine[i * 2] = sf::Vertex(sf::Vector2f{ 0, (float)(i * tileSize) });
        m_rowLine[i * 2 + 1] = sf::Vertex(sf::Vector2f{ screen_Width, (float)(i * tileSize) });
    }

    int id = 0;

    for (int i = 0; i < m_rows; i++)
    {
        for (int j = 0; j < m_cols; j++)
        {
            NodeData nodeData = NodeData{ id,sf::Vector2f((float)((j * tileSize) + ((float)tileSize / 2)), (float)(i * tileSize) + ((float)tileSize / 2)) };
            addNode(nodeData, id++);
        }
    }


    std::ifstream levelData("level1.json");
    nlohmann::json levelJson = nlohmann::json::parse(levelData);
    auto pathfinding = levelJson["pathfinding"];

    // Store the list of waypoints
    for (auto& waypoint : pathfinding["waypoints"])
        m_waypoints.push_back(waypoint);

    // Store each path between each waypoint
    for (auto it = pathfinding["paths"].begin(); it != pathfinding["paths"].end(); ++it)
    {
        std::vector<int> path;
        for (auto &tile: it.value())
            path.push_back(tile);

        m_ucsPaths.insert({it.key(), path});
    }
}

/**
 * Destructor, This deletes every node
 *
 * @tparam NodeType
 * @tparam ArcType
 */
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

/**
 * Draw the grid on the screen (debug purposes)
 *
 * @tparam NodeType
 * @tparam ArcType
 */
template<typename NodeType, typename ArcType>
void Graph<NodeType, ArcType>::toggleDraw()
{
    m_gridDraw = !m_gridDraw;
}

template<typename NodeType, typename ArcType>
void Graph<NodeType, ArcType>::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    if (m_gridDraw)
    {
        for (auto& path : m_pathDraw)
            target.draw(path);

        target.draw(m_colLine, 96, sf::Lines);
        target.draw(m_rowLine, 60, sf::Lines);
        for (int i = 0; i < m_size; i++)
        {
            target.draw(m_gridText[i]);
        }

        sf::RectangleShape debugRect;
        debugRect.setFillColor(sf::Color(255, 0, 0, 127));
        debugRect.setSize(sf::Vector2f(tileSize, tileSize));
        int m_id = 0;
        for (int i = 0; i < m_rows; i++)
        {
            for (int j = 0; j < m_cols; j++)
            {
                if (!m_nodes.at(m_id)->m_data.isPassable)
                {
                    debugRect.setPosition(j * tileSize, i * tileSize);
                    target.draw(debugRect);
                }
                m_id++;
            }
        }
    }
}

template<typename NodeType, typename ArcType>
void Graph<NodeType, ArcType>::debug(sf::Font& t_font)
{
    m_gridTextElement.setFont(t_font);
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

/**
 * Get a 2-Dimensional vector representing the graph/grid and containing each Node
 *
 * @tparam NodeType
 * @tparam ArcType
 * @return 2-Dimensional vector of nodes
 */
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

/**
 * This adds a node at a given index in the graph.
 *
 * @tparam NodeType
 * @tparam ArcType
 * @param data data to store in the node.
 * @param index index to store the node.
 * @return true if successful
 */
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

/**
 *
 * @tparam NodeType
 * @tparam ArcType
 * @param data data update in the node
 * @param index index of the node to update.
 * @return
 */
template<typename NodeType, class ArcType>
bool Graph<NodeType, ArcType>::updateNode(NodeData data, int index)
{
    // find out if a node does not exist at that index.
    if (m_nodes.at(index) == nullptr)
        return false;

    m_nodes.at(index)->m_data = data;

    return true;
}

/**
 * Get the list of node of the graph
 *
 * @tparam NodeType
 * @tparam ArcType
 * @return vector containing the node list of the graph
 */
template<typename NodeType, class ArcType>
std::vector<GraphNode<NodeType, ArcType>*> Graph<NodeType, ArcType>::getNodes()
{
    return m_nodes;
}

/**
 * This removes a node from the graph
 *
 * @tparam NodeType
 * @tparam ArcType
 * @param index The index of the node to return.
 */
template<class NodeType, class ArcType>
void Graph<NodeType, ArcType>::removeNode(int index)
{
    // Only proceed if node does exist.
    if (nullptr != m_nodes.at(index))
    {
        // now find every arc that points to the node that
        // is being removed and remove it.
        Arc* arc;

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

/**
 * Adds an arc from the first index to the second index with the specified weight.
 *
 * @tparam NodeType
 * @tparam ArcType
 * @param from originating node index
 * @param to ending node index
 * @param weight weight of the arc
 * @return true on success.
 */
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

/**
 * This removes the arc from the first index to the second index
 *
 * @tparam NodeType
 * @tparam ArcType
 * @param from originating node index.
 * @param to ending node index.
 */
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

/**
 * Gets a pointer to an arc from the first index to the second index.
 *
 * @tparam NodeType
 * @tparam ArcType
 * @param from originating node index.
 * @param to ending node index.
 * @return pointer to the arc, or 0 if it doesn't exist.
 */
template<class NodeType, class ArcType>
GraphArc<NodeType, ArcType>* Graph<NodeType, ArcType>::getArc(int from, int to)
{
    Arc* arc = 0;
    // make sure the to and from nodes exist
    if (nullptr != m_nodes.at(from) && nullptr != m_nodes.at(to))
    {
        arc = m_nodes.at(from)->getArc(m_nodes.at(to));
    }

    return arc;
}

/**
 * This clears every mark on every node.
 *
 * @tparam NodeType
 * @tparam ArcType
 */
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

        int cost = 60; // Because width/height of a cell is 60px
        if (direction == 0 || direction == 2 || direction == 6 || direction == 8) // Diagonal
            cost = 85; // Because diagonal of a square is sqrt(width or height) so here its sqrt(60)

        int n_row = row + ((direction % 3) - 1); // Neighbor row
        int n_col = col + ((direction / 3) - 1); // Neighbor column

        // Check the bounds:
        if (n_row >= 0 && n_row < vectorGrid.size() && n_col >= 0 && n_col < vectorGrid[n_row].size())
        {
            // A valid neighbor:
            if (vectorGrid[n_row][n_col].m_data.isPassable)
            {
                neighbours.insert({ vectorGrid[n_row][n_col].m_data.id, cost });
            };
        }
    }

    return neighbours;
}

template<class NodeType, class ArcType>
void Graph<NodeType, ArcType>::aStar(Node* start, Node* dest, std::vector<Node*>& path)
{
    if (start == nullptr || dest == nullptr) return;

    std::priority_queue<Node *, std::vector<Node *>, Comparer<NodeType, ArcType>> pq;

    for (int i = 0; i < m_nodes.size(); i++)
    {
        m_nodes.at(i)->setPrevious(nullptr);
        int x = m_nodes.at(i)->m_data.position.x;
        int y = m_nodes.at(i)->m_data.position.y;
        int goalX = dest->m_data.position.x;
        int goalY = dest->m_data.position.y;

        m_nodes.at(i)->m_data.h = sqrt(pow(goalX - x, 2) + pow(goalY - y, 2)); // Calculate euclidean distance to goal
        m_nodes.at(i)->m_data.g = std::numeric_limits<int>::max() - 10000;
    }
    start->m_data.g = 0;
    start->m_data.h = 0;

    pq.push(start);
    start->setMarked(true);

    while (!pq.empty() && pq.top() != dest)
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
                int distToChild = arcWeight + pq.top()->m_data.g;

                if (distToChild < child->m_data.g)
                {
                    child->m_data.g = distToChild;
                    child->setPrevious(pq.top());
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

    Node *temp = dest;

    for (; temp != nullptr; temp = temp->previous())
    {
        path.push_back(temp);
    }

}

template<typename NodeType, class ArcType>
std::vector<int> Graph<NodeType, ArcType>::getWaypoints()
{
    return m_waypoints;
}

template<typename NodeType, class ArcType>
std::map<std::string, std::vector<int>> Graph<NodeType, ArcType>::getUCSPaths()
{
    return m_ucsPaths;
}

template<typename NodeType, class ArcType>
int Graph<NodeType, ArcType>::getClosestWaypoint(sf::Vector2f pos)
{
    float lowestDistance = std::numeric_limits<float>::max();
    int closestNode = -1;
    for (auto& waypoint : m_waypoints)
    {
        sf::Vector2f waypointPos = nodeIndex(waypoint)->m_data.position;
        float distance = Utils::getDistanceBetweenPoints(waypointPos, pos);

        if(distance < lowestDistance)
        {
            lowestDistance = distance;
            closestNode = waypoint;
        }
    }

    return closestNode;
}

/**
 * Get one of the pre-calculated UCS Path
 *
 * @tparam NodeType Data stored in the Node
 * @tparam ArcType Type of the Arc
 * @param from index of the node where the path should come from
 * @param to index of the node where the path should go
 * @return the path or an empty vector if not found
 */
template<typename NodeType, class ArcType>
std::vector<int> Graph<NodeType, ArcType>::getUCSPath(int from, int to)
{
    auto ucsPaths = getUCSPaths();
    auto path = ucsPaths.find(std::to_string(from) + "-" + std::to_string(to));
    if (path == ucsPaths.end()) return {};

    return path->second;
}

