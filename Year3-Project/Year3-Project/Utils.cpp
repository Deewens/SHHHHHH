#include "Utils.h"

#include <iostream>

/// <summary>
/// Generate a random number between two values
/// </summary>
int Utils::generateRandomNumber(int from, int to)
{
    std::random_device rd;
    std::minstd_rand gen(rd());

    std::uniform_int_distribution<> rand(from, to);
    return rand(gen);
}

void Utils::playRandomSound(std::vector<sf::Sound*> &sounds)
{
    int random = generateRandomNumber(0, (int) sounds.size()-1);
    sf::Sound* sound = sounds.at(random);
    sound->play();
}

float Utils::getDistanceBetweenPoints(sf::Vector2f p, sf::Vector2f q)
{
    return sqrt(pow(q.x - p.x, 2) + pow(q.y - p.y, 2));
}

/**
 * Get the neighbours of the given node
 *
 * @param grid get the neighbours of the node from this grid
 * @param node the node from which to get the neighbours
 * @return the list of node, neighbours of the given node
 */
std::vector<int> Utils::getNeighbours(Grid grid, int node)
{
    // Get the row and col index of the node
    auto vectorGrid = grid.getGrid();

    int row = 0;
    int col = 0;
    for (int i = 0; i < vectorGrid.size(); i++)
    {
        for (int j = 0; j < vectorGrid[i].size(); j++)
        {
            if (vectorGrid[i][j].getId() == node)
            {
                row = i;
                col = j;
                break;
            }
        }
    }

    // Get the neighbours of the node
    std::vector<int> neighbours;

    // List all neighbors:
    for (int direction = 0; direction < 9; direction++) {
        if (direction == 4) continue; // Skip 4, this is ourself.

        int n_row = row + ((direction % 3) - 1); // Neighbor row
        int n_col = col + ((direction / 3) - 1); // Neighbor column

        // Check the bounds:
        if (n_row >= 0 && n_row < vectorGrid.size() && n_col >= 0 && n_col < vectorGrid[n_row].size()) {
            // A valid neighbor:
            if (vectorGrid[n_row][n_col].isPassable()) neighbours.push_back(vectorGrid[n_row][n_col].getId());
        }
    }

    return neighbours;
}
