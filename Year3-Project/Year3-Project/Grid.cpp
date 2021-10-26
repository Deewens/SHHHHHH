#include "Grid.h"
Grid::Grid() : m_rows(0), m_cols()
{
}

Grid::Grid(int t_rows, int t_cols) : m_rows(t_rows), m_cols(t_cols)
{
	int size = t_rows * t_cols;	

	for (int i = 0; i < m_cols; i++)
	{
		m_colLine[i * 2] = sf::Vertex(sf::Vector2f{ (float)(i * tileSize), 0 });
		m_colLine[i * 2 + 1] = sf::Vertex(sf::Vector2f{ (float)(i * tileSize), screem_Height });
	}

	for (size_t i = 0; i < m_rows; i++)
	{
		m_rowLine[i * 2] = sf::Vertex(sf::Vector2f{ 0,(float)(i * tileSize) });
		m_rowLine[i * 2 + 1] = sf::Vertex(sf::Vector2f{ screen_Width ,(float)(i * tileSize) });
	}
}

void Grid::render(sf::RenderWindow& t_window)
{
	if (m_gridDraw)
	{
		t_window.draw(m_colLine, 96, sf::Lines);
		t_window.draw(m_rowLine, 60, sf::Lines);
	}
}

void Grid::update()
{
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::G))
	{
		m_gridDraw = true;
	}
	
}