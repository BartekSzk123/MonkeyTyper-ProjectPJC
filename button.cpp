#include "Button.hpp"

Button::Button(const std::string &chosenText, const sf::Font &chosenFont, const sf::Vector2f &pos, const std::function<void()> onClick)
    : font(chosenFont), text(font, chosenText, 25), shape(sf::Vector2f(200, 50)), click(onClick) {
    text.setFillColor(sf::Color::Black);
    text.setOrigin(sf::Vector2f(
            text.getLocalBounds().position.x + text.getLocalBounds().size.x / 2,
            text.getLocalBounds().position.y + text.getLocalBounds().size.y / 2)
    );
    text.setPosition(pos);

    shape.setFillColor(sf::Color::White);
    shape.setOrigin(sf::Vector2f(
            shape.getLocalBounds().position.x + shape.getLocalBounds().size.x / 2,
            shape.getLocalBounds().position.y + shape.getLocalBounds().size.y / 2)
    );
    shape.setPosition(pos);
}

void Button::checkClick(const sf::Vector2f &mousePos) {
    if (shape.getGlobalBounds().contains(mousePos)) {
        if (click) click();
    }
}

void Button::setFont(const sf::Font &newFont) {
    font = newFont;
    text.setFont(font);
}

void Button::setColor(const sf::Color &newColor) {
    shape.setFillColor(newColor);
}

void Button::draw(sf::RenderTarget &target, sf::RenderStates states) const {
    target.draw(shape, states);
    target.draw(text, states);
}
