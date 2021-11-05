#pragma once
#include <SFML/Graphics.hpp>
#include "Globals.h"
#include<iostream>

class Grid : public sf::Drawable
{
public:
	Grid();
	Grid(int t_m_rows, int t_m_cols);

	void render(sf::RenderWindow& t_window);

	void update();

private:
	int m_rows;
	int m_cols;

	sf::Vertex m_colLine[96];
	sf::Vertex m_rowLine[60];

	bool m_gridDraw = false;

    void draw(sf::RenderTarget &target, sf::RenderStates states) const final;

};

