#pragma once

#include<vector>
#include<queue>
#include <SFML/Graphics.hpp>
#include "Globals.h"
#include<iostream>

class Cell
{
public:

	Cell()
	{
		cellCreator();
	}
	//Cell(int t_cellId = -1) : m_id(t_cellId), m_parentCellId(-1)
	//{
	//}// Constructor function to accept and store the cell id	

	void addNeighbour(int t_cellId)
	{
		m_neighbours.push_back(t_cellId);
	}	// To add a neighbour

	std::vector<int>& neighbours()
	{
		return m_neighbours;
	}
	// To return the list of neighbours
	void setMarked(bool t_mark)
	{
		m_isMarked = t_mark;
	}

	int id() const
	{
		return m_id;
	}

	void setParentCellId(int t_cellId)
	{
		m_parentCellId = t_cellId;
	}

	int getParentCellId()
	{
		return m_parentCellId;
	}

	bool isOpen() const
	{
		return !m_isMarked && m_isPassable;
	}

	void setPassable(bool t_mark)
	{
		m_isPassable = false;
	}

	void cellCreator()
	{
		int m_cId = 0;

		for (int i = 0; i < m_gridCols; i++)
		{
			for (int j = 0; j < m_gridRows; j++)
			{
				m_cellId.push_back(m_cId);
				m_cId++;
			}
		}
	}

	int cellIdFinder(sf::Vector2f t_targetLocation)
	{
		std::cout << std::to_string(m_id)<<std::endl;

		return m_id = floor(t_targetLocation.x / tileSize) + (floor(t_targetLocation.y / tileSize) * m_gridCols);
	}

private:

	int m_id; // The id for this cell

	std::vector<int> m_cellId;

	std::vector<int> m_neighbours; // A one dimensional array containing all the neighbours
	bool m_isMarked{ false };
	bool m_isPassable{ true };
	int m_parentCellId;
};
