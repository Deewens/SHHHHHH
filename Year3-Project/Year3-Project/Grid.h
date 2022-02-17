#pragma once
#include <SFML/Graphics.hpp>
#include "Globals.h"
#include<iostream>
#include<string>
#include<vector>
#include <fstream>
#include <string>
#include "Cell.h"


class Grid : public sf::Drawable
{
public:
	Grid();
	Grid(int t_m_rows, int t_m_cols);


	// Resets the 'marks' (to false) on each cell
	void reset();

	void markImpassableCell(int t_cellId);

	//void render(sf::RenderWindow& t_window);

	void toggleDraw();

	void debug();

private:
	int m_rows;
	int m_cols;

	sf::Vertex m_colLine[96];
	sf::Vertex m_rowLine[60];

	bool m_gridDraw = false;

    void draw(sf::RenderTarget &target, sf::RenderStates states) const final;


	int m_size = 0;	

	sf::Font m_gridFont;

	sf::Text m_gridTextElement;
	std::vector<sf::Text> m_gridText;

	std::vector<Cell> m_cells;
	void neighbours();


	bool m_debugDraw = false;
};

