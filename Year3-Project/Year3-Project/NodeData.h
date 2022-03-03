#pragma once

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
	int g{}; // Cost so far to reach the goal
    int h{}; // Estimated cost to the goal from n
};
