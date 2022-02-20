#pragma once
#include  <string>

// ---------------------------------------------------------------------
//  Name:           NodeData
//  Description:    A simple struct to encapsulate all the node data.
//                  Add or remove fields as required.               
// ---------------------------------------------------------------------
struct NodeData
{
	int id = 0;
	sf::Vector2f position;
	bool isPassable = true;
	int cost;
    int heuristic;
};
