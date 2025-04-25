#pragma once
#include <SFML/Graphics.hpp>
#include <functional>
#include <string>

struct Button : sf::Drawable {
    sf::Text text;
    sf::Font font;
    sf::RectangleShape shape;
    std::function<void()> click;

    Button(const std::string &chosenText, const sf::Font &chosenFont, const sf::Vector2f &pos,
           const std::function<void()> onClick);

    void checkClick(const sf::Vector2f &mousePos);

    void setFont(const sf::Font &newFont);

    void setColor(const sf::Color &newColor);

    void draw(sf::RenderTarget &target, sf::RenderStates states) const override;
};
