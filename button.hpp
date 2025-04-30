#pragma once
#include <SFML/Graphics.hpp>
#include <functional>
#include <string>

struct Button : sf::Drawable {
    sf::Text text;
    sf::Font font;
    sf::RectangleShape shape;
    std::function<void()> click;
    bool isVisible = true;

    Button(const std::string &chosenText,const int &charSize, const sf::Font &chosenFont,const sf::Vector2f &size,
        const sf::Vector2f &pos, const std::function<void()> onClick);

    void checkClick(const sf::Vector2f &mousePos);

    void setFont(const sf::Font &newFont);

    void setVisibility(bool newVisibility);

    void setShapeColor(const sf::Color &newColor);

    void setTextColor(const sf::Color &newColor);

    void setText(const std::string &newText);

    void setSizeOfShape(const int &newSizeX,const int &newSizeY);

    void setNewFunction(const std::function<void()> &newOnClick);

    void draw(sf::RenderTarget &target, sf::RenderStates states) const override;
};
