#include "Grid.h"
Grid::Grid() : m_rows(0), m_cols()
{
}

Grid::Grid(int t_rows, int t_cols) : m_rows(t_rows), m_cols(t_cols)
{
	m_size = t_rows * t_cols;	

	for (int i = 0; i < m_cols; i++)
	{
		m_colLine[i * 2] = sf::Vertex(sf::Vector2f{ (float)(i * tileSize), 0 });
		m_colLine[i * 2 + 1] = sf::Vertex(sf::Vector2f{ (float)(i * tileSize), screen_Height });
	}

	for (int i = 0; i < m_rows; i++)
	{
		m_rowLine[i * 2] = sf::Vertex(sf::Vector2f{ 0,(float)(i * tileSize) });
		m_rowLine[i * 2 + 1] = sf::Vertex(sf::Vector2f{ screen_Width ,(float)(i * tileSize) });
	}
}

void Grid::toggleDraw()
{
	m_gridDraw = !m_gridDraw;
}


void Grid::draw(sf::RenderTarget &target, sf::RenderStates states) const
{
    if (m_gridDraw)
    {
        target.draw(m_colLine, 96, sf::Lines);
        target.draw(m_rowLine, 60, sf::Lines);
		//for (int i = 0; i < m_size; i++)
		//{
			//target.draw(m_gridText[i]);
		//}
    }
}
void Grid::debug()
{
	if(!m_gridFont.loadFromFile("ASSETS\\FONTS\\ariblk.ttf"))
	{
		std::cout << "Problem loading Grid Font" << std::endl;
	}
	m_gridTextElement.setFont(m_gridFont);
	m_gridTextElement.setCharacterSize(20);
	m_gridTextElement.setFillColor(sf::Color::Black);
	
	int m_id = 0;

	for (int i = 0; i < m_cols; i++)
	{
		for(int j = 0; j < m_rows; j++)
		{
			m_gridTextElement.setString(std::to_string(m_id));
			m_gridTextElement.setPosition(j * tileSize, i* tileSize);
			m_gridText.push_back(m_gridTextElement);
			m_id++;
		}
	}
}
