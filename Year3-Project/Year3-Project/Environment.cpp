#include <iostream>
#include "Environment.h"

Environment::Environment(sf::RectangleShape& t_rect, int t_tileCode, int t_rows, int t_cols, float t_rotation,
                         bool t_impassable) : m_rect(t_rect), m_rotation(t_rotation), m_impassable(t_impassable)
{
    m_rect.setSize(sf::Vector2f(tileSize, tileSize));
    m_rect.setOrigin(sf::Vector2f(tileSize / 2, tileSize / 2));
	float col = t_tileCode % t_cols;
	float row = (t_tileCode - col) / t_cols;
	col = (col * tileSize) + (tileSize / 2);
	row = (row * tileSize) + (tileSize / 2);
    m_rect.setPosition(sf::Vector2f(col, row));
}

Environment::Environment(sf::Texture& t_texture, const std::string& t_sprite, int t_tileCode, int t_rows, int t_cols,
                         float t_rotation, bool t_impassable) :
                         m_sprite(t_texture), m_rotation(t_rotation), m_impassable(t_impassable)
{
    std::ifstream spriteSheetData("ASSETS/IMAGES/sprite_sheets/data/ground_sprite_sheet.json");
    nlohmann::json json;
    spriteSheetData >> json;

    nlohmann::json frame = json["frames"][t_sprite]["frame"];
    int x = frame["x"];
    int y = frame["y"];
    float width = frame["w"];
    float height = frame["h"];

    m_sprite.setTextureRect(sf::IntRect(x, y, width, height));
    m_sprite.setScale((float)tileSize/width, (float)tileSize/height);

    m_sprite.setOrigin(sf::Vector2f(tileSize / 2, tileSize / 2));
    m_sprite.setRotation(m_rotation);
    float col = t_tileCode % t_cols;
    float row = (t_tileCode - col) / t_cols;
    col = (col * tileSize) + (tileSize / 2);
    row = (row * tileSize) + (tileSize / 2);
    m_sprite.setPosition(sf::Vector2f(col, row));
}

void Environment::render(sf::RenderWindow& t_window)
{
    t_window.draw(m_sprite);
    t_window.draw(m_rect);
}

sf::FloatRect Environment::getCollisionRect()
{
	return m_rect.getGlobalBounds();
}

bool Environment::isImpassable() const
{
    return m_impassable;
}
