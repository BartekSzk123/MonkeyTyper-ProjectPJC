#include "uiElement.hpp"

uiElement::uiElement(const std::string &chosenText, const int &charSize, const sf::Font &chosenFont, const sf::Vector2f &size,
                     const sf::Vector2f &pos, const std::function<void()> onClick)
    : font(chosenFont), text(font, chosenText, charSize), shape(sf::Vector2f(size)), click(onClick) {
    text.setFillColor(sf::Color::Black);
    text.setOrigin(sf::Vector2f(
            text.getLocalBounds().position.x + text.getLocalBounds().size.x / 2,
            text.getLocalBounds().position.y + text.getLocalBounds().size.y / 2)
    );
    text.setPosition(pos);

    shape.setFillColor({97, 39, 24});
    shape.setOrigin(sf::Vector2f(
            shape.getLocalBounds().position.x + shape.getLocalBounds().size.x / 2,
            shape.getLocalBounds().position.y + shape.getLocalBounds().size.y / 2)
    );
    shape.setPosition(pos);
}

void uiElement::checkClick(const sf::Vector2f &mousePos) {
    if (!isVisible) {
        return;
    }

    if (shape.getGlobalBounds().contains(mousePos)) {
        if (click) click();
    }
}

void uiElement::setFont(const sf::Font &newFont) {
    font = newFont;
    text.setFont(font);
}

void uiElement::setShapeColor(const sf::Color &newColor) {
    shape.setFillColor(newColor);
}

void uiElement::setTextColor(const sf::Color &newColor) {
    text.setFillColor(newColor);
}

void uiElement::setVisibility(bool newVisibility) {
    isVisible = newVisibility;
}

void uiElement::setText(const std::string &newText) {
    text.setString(newText);
}

void uiElement::setSizeOfShape(const int &newSizeX, const int &newSizeY) {
    shape.setSize(sf::Vector2f(newSizeX, newSizeY));
}

void uiElement::setNewFunction(const std::function<void()> &newOnClick) {
    click = newOnClick;
}

void uiElement::draw(sf::RenderTarget &target, sf::RenderStates states) const {
    target.draw(shape, states);
    target.draw(text, states);
}
