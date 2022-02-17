#include "Grid.h"

Grid::Grid() : m_rows(0), m_cols()
{
}

Grid::Grid(int t_rows, int t_cols) : m_rows(t_rows), m_cols(t_cols)
{
	int size = t_rows * t_cols;
	for (int index = 0; index < size; index++)
	{
		m_cells.emplace_back(Cell(index));
	}
	neighbours();	
}

//std::vector<int> Grid::breadthFirst(int t_startCellId, int t_destCellId)
//{
//	bool goalReached = false;
//	std::queue<Cell> cellQueue;
//
//	Cell startCell = m_cells.at(t_startCellId);
//	cellQueue.emplace(startCell);
//	m_cells.at(t_startCellId).setMarked(true);
//	m_cells.at(t_startCellId).setParentCellId(-1);
//	while (!cellQueue.empty() && !goalReached)
//	{
//		for (int index = 0; index < cellQueue.front().neighbours().size() && !goalReached; index++)
//		{
//
//			int childCellId = cellQueue.front().neighbours().at(index);
//
//			Cell& childCell = m_cells.at(childCellId);
//
//			if (childCellId == t_destCellId)
//			{
//				goalReached = true;
//				m_cells.at(t_destCellId).setParentCellId(cellQueue.front().id());
//			}
//			else if (childCell.isOpen())
//			{
//				m_cells.at(childCell.id()).setParentCellId(cellQueue.front().id());
//				childCell.setMarked(true);
//				cellQueue.emplace(childCell);
//			}
//		}
//		cellQueue.pop();		
//	}
//	reset();
//	std::vector<int> path;
//	int currentCell = t_destCellId;
//	while (m_cells[currentCell].getParentCellId() != -1)
//	{
//		path.push_back(currentCell);
//		currentCell = m_cells[currentCell].getParentCellId();
//	}
//	path.push_back(currentCell);
//	return path;
//}

void Grid::reset()
{
	int size = m_rows * m_cols;
	for (int index = 0; index < size; index++)
	{
		m_cells[index].setMarked(false);
	}
}

void Grid::markImpassableCell(int t_cellId)
{
	m_cells.at(t_cellId).setPassable(false);

	for (int index = 0; index < m_cells.at(t_cellId).neighbours().size(); index++)
	{
		int neighbourCellId = m_cells.at(t_cellId).neighbours().at(index);
		m_cells.at(neighbourCellId).setPassable(false);
	}
}
void Grid::neighbours()
{
	for (int row = 0; row < m_rows; row++)
	{
		for (int col = 0; col < m_cols; col++)
		{
			for (int direction = 0; direction < 9; direction++)
			{
				if (direction == 4)continue;

				int n_row = row + ((direction % 3) - 1); //neighbor row
				int n_col = col + ((direction / 3) - 1); //neighbor colum

				if (n_row >= 0 && n_row < m_rows && n_col >= 0 && n_col < m_cols)
				{
					int index = row * m_cols + col;

					int indexOfNeighbor = n_row * m_cols + n_col;

					m_cells.at(index).addNeighbour(indexOfNeighbor);
				}
			}
		}
	}
}
