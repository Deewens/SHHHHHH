#pragma once
#include <SFML/Graphics.hpp>
#include "Globals.h"

class MenuButton
{
public:

    sf::RectangleShape shape;
    sf::Text text;
    ButtonState state = ButtonState::READY;
    sf::Vector2f downPos;
    sf::Vector2f readyPos;
    bool pressed = false;
    sf::Color curFillColor;
    void Init(sf::Vector2f pos, sf::Font& font, sf::Vector2f t_size, sf::Vector2f t_offset)
    {
        pressed = false;
        readyPos = pos;
        curFillColor = sf::Color::Red;
        downPos = sf::Vector2f(pos.x - 10, pos.y + 10);
        shape.setSize(t_size);
        shape.setPosition(pos);
        text.setFont(font);
        text.setPosition(shape.getPosition().x + t_offset.x, shape.getPosition().y + t_offset.y);

        UpdateUI();
    }
    void UpdateUI()
    {
        shape.setFillColor(curFillColor);
    }
    void Update(sf::Vector2f mousePos, sf::RenderWindow& t_window)
    {
        pressed = false;
        if (state == ButtonState::READY)
        {
            shape.setPosition(readyPos);
            curFillColor = sf::Color::Red;
            if (getButtonRect(t_window).contains(mousePos))
            {
                state = ButtonState::OVER;
            }
        }
        if (state == ButtonState::OVER)
        {
            curFillColor = sf::Color::Yellow;
            if (getButtonRect(t_window).contains(mousePos))
            {
                if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
                {
                    state = ButtonState::DOWN;
                }
            }
            else
            {
                state = ButtonState::READY;
            }
        }
        if (state == ButtonState::DOWN)
        {
            curFillColor = sf::Color::Black;
            shape.setPosition(downPos);
            if (getButtonRect(t_window).contains(mousePos))
            {
                if (!sf::Mouse::isButtonPressed(sf::Mouse::Left))
                {
                    state = ButtonState::READY;
                    pressed = true;
                }
            }
            else
            {
                state = ButtonState::READY;
            }
        }
        UpdateUI();
    }
private:
    sf::FloatRect getButtonRect(sf::RenderWindow& t_window)
    {
        sf::FloatRect button = shape.getGlobalBounds();
        sf::Vector2u window = t_window.getSize();

        button.left = window.x * (button.left / screen_Width);
        button.top = window.y * (button.top / screen_Height);
        button.width = window.x * (button.width / screen_Width);
        button.height = window.y * (button.height / screen_Height);

        return button;
    }
};

