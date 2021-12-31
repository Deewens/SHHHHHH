#pragma once
#include <SFML/Graphics.hpp>
#include "Globals.h"
#include "NodeData.h"
#include<iostream>
#include<string>
#include<vector>
#include <set>


class Grid : public sf::Drawable
{
public:
	Grid();
	Grid(int t_m_rows, int t_m_cols);

	void render(sf::RenderWindow& t_window);

	void toggleDraw();

	void debug();

    std::vector<std::vector<NodeData>>& getGrid();

private:
	int m_rows;
	int m_cols;

    std::vector<std::vector<NodeData>> m_grid; // Store the grid in a 2d array in the good order

	sf::Vertex m_colLine[96];
	sf::Vertex m_rowLine[60];

	bool m_gridDraw = false;

    void draw(sf::RenderTarget &target, sf::RenderStates states) const final;


	int m_size = 0;	

	sf::Font m_gridFont;

	sf::Text m_gridTextElement;
	std::vector<sf::Text> m_gridText;
};

