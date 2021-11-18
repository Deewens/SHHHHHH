#include "Environment.h"

Environment::Environment()
{
	rect.setSize(sf::Vector2f(50, 50));
	rect.setOrigin(sf::Vector2f(25, 25));
	rect.setFillColor(sf::Color::Red);
	rect.setPosition(sf::Vector2f(350, 150));
}

Environment::Environment(int t_tileCode, int t_rows, int t_cols, int t_tileTypeCode)
{
	rect.setSize(sf::Vector2f(tileSize, tileSize));
	rect.setOrigin(sf::Vector2f(tileSize / 2, tileSize / 2));
	rect.setFillColor(sf::Color::Red);
	float col = t_tileCode % t_cols;
	float row = (t_tileCode - col) / t_cols;
	col = (col * tileSize) + (tileSize / 2);
	row = (row * tileSize) + (tileSize / 2);
	rect.setPosition(sf::Vector2f(col, row));
}

Environment::Environment(sf::Texture& t_texture, std::string t_sprite, int t_tileCode, int t_rows, int t_cols, int t_tileTypeCode,
                         float rotation) : m_sprite(t_texture)
{
    std::ifstream spriteSheetData("ASSETS/IMAGES/sprite_sheets/data/ground_sprite_sheet.json");
    nlohmann::json json;
    spriteSheetData >> json;

    nlohmann::json frame = json["frames"][t_sprite]["frame"];
    int x = frame["x"];
    int y = frame["y"];
    int width = frame["w"];
    int height = frame["h"];

    m_sprite.setTextureRect(sf::IntRect(x, y, width, height));
    m_sprite.setScale(0.46875, 0.46875);

    m_sprite.setOrigin(sf::Vector2f(width / 2, height / 2));
    m_sprite.setRotation(rotation);
    float col = t_tileCode % t_cols;
    float row = (t_tileCode - col) / t_cols;
    col = (col * tileSize) + (tileSize / 2);
    row = (row * tileSize) + (tileSize / 2);
    m_sprite.setPosition(sf::Vector2f(col, row));
}

void Environment::render(sf::RenderWindow& t_window)
{
	t_window.draw(rect);
    t_window.draw(m_sprite);
}

sf::FloatRect Environment::getCollisionRect()
{
	return rect.getGlobalBounds();
}

void Environment::impassable()
{

}
