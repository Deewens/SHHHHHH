#include <iostream>
#include "Environment.h"

Environment::Environment(sf::RectangleShape& t_rect, int t_tileCode, int t_rows, int t_cols, float t_rotation,
                         bool t_impassable) : m_rect(t_rect), m_rotation(t_rotation), m_impassable(t_impassable), m_tileCode(t_tileCode)
{
    m_rect.setSize(sf::Vector2f(tileSize, tileSize));
    m_rect.setOrigin(sf::Vector2f(tileSize / 2, tileSize / 2));
	float col = t_tileCode % t_cols;
	float row = (t_tileCode - col) / t_cols;
	col = (col * tileSize) + (tileSize / 2);
	row = (row * tileSize) + (tileSize / 2);
    m_rect.setPosition(sf::Vector2f(col, row));
    m_noiseLevel = NoiseLevels::WHITE;
}

Environment::Environment(sf::Texture& t_texture, const std::string& t_sprite, int t_tileCode, int t_rows, int t_cols,
                         float t_rotation, bool t_impassable) :
                         m_sprite(t_texture), m_rotation(t_rotation), m_impassable(t_impassable), m_tileCode(t_tileCode)
{
    std::ifstream spriteSheetData("ASSETS/TileSheet/spritesheet.json");
    nlohmann::json json;
    spriteSheetData >> json;

    nlohmann::json frame = json["frames"][t_sprite];
    int x = frame["frame"]["x"];
    int y = frame["frame"]["y"];
    float width = frame["frame"]["w"];
    float height = frame["frame"]["h"];

    m_sprite.setTextureRect(sf::IntRect(x, y, width, height));
    m_sprite.setScale((float)tileSize/width, (float)tileSize/height);

    m_sprite.setOrigin(sf::Vector2f(width / 2, height / 2));
    m_sprite.setRotation(m_rotation);
    float col = t_tileCode % t_cols;
    float row = (t_tileCode - col) / t_cols;
    col = (col * tileSize) + (tileSize / 2);
    row = (row * tileSize) + (tileSize / 2);
    m_sprite.setPosition(sf::Vector2f(col, row));

    std::cout << t_sprite << std::endl;
    int noise = frame["NoiseLevels"];
    switch (noise)
    {
    case 0:
        m_noiseLevel = NoiseLevels::WHITE;
        break;
    case 1:
        m_noiseLevel = NoiseLevels::GREEN;
        break;
    case 2:
        m_noiseLevel = NoiseLevels::YELLOW;
        break;
    case 3:
        m_noiseLevel = NoiseLevels::RED;
        break;
    default:
        m_noiseLevel = NoiseLevels::WHITE;
        break;
    }
}

void Environment::render(sf::RenderWindow& t_window)
{
    t_window.draw(m_sprite);
    t_window.draw(m_rect);
}

sf::FloatRect Environment::getCollisionRect()
{
	return m_sprite.getGlobalBounds();
}

bool Environment::isImpassable() const
{
    return m_impassable;
}

int Environment::getTileCode() const
{
    return m_tileCode;
}

NoiseLevels Environment::getNoise()
{
    return m_noiseLevel;
}
