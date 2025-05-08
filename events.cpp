#include "events.hpp"

auto onClose(sf::Event::Closed const& event, sf::RenderWindow& window) -> void {
    window.close();
}

auto mouseClick(sf::Event::MouseButtonPressed const& event,std::vector<Button*> &buttons) -> void {
    if (event.button == sf::Mouse::Button::Left) {
        for (auto &b: buttons) {
            auto clicked = b->shape.getGlobalBounds().contains(static_cast<sf::Vector2f>(event.position));

            if (clicked) {
                b->checkClick(sf::Vector2f(event.position.x, event.position.y));
            }
        }
    }
}

auto mouseHover(sf::Event::MouseMoved const& event,std::vector<Button*> &buttons) -> void {
    for (auto &b: buttons) {
        auto hovered = b->shape.getGlobalBounds().contains(static_cast<sf::Vector2f>(event.position));

        if (hovered && b->isVisible) {
            b->setShapeColor({205, 133, 63});
        } else {
            b->setShapeColor({97, 39, 24});
        }
    }
}


auto keyPressed(sf::Event::KeyPressed const& event, Button &wordsSpeedButton, Button &charSizeButton, Button &fontButton) -> void {

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S) && sf::Keyboard::isKeyPressed(sf::Keyboard::Key::LControl)) {
        wordsSpeedButton.click();
    }

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::C) && sf::Keyboard::isKeyPressed(sf::Keyboard::Key::LControl)) {
        charSizeButton.click();
    }

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::F) && sf::Keyboard::isKeyPressed(sf::Keyboard::Key::LControl)) {
        fontButton.click();
    }

}

auto textEntered(sf::Event::TextEntered const &event, std::string& input,
    std::vector<sf::Text>& generatedWords, int& score, int& wordsCounter,
    Button &scoreBar, Button &currentTyping, GameStatus status)->void {

    if (event.unicode < 128 && status == GameStatus::GameStart) {
        auto charEntered = static_cast<char>(event.unicode);

        if (charEntered == '\n') {
            for (auto iterator = generatedWords.begin(); iterator != generatedWords.end(); ++iterator) {
                if (input == iterator->getString()) {
                    score += iterator->getString().getSize() * 2;
                    wordsCounter++;
                    iterator = generatedWords.erase(iterator);
                    scoreBar.setText(
                        "SCORE: " + std::to_string(score) + "\tTYPED WORDS: " + std::to_string(wordsCounter));
                }
            }
            input.clear();
            currentTyping.setText("INPUT: ");
        } else if (charEntered == '\b' && !input.empty()) {
            input.pop_back();
            currentTyping.setText(
                "INPUT:  " + input);
        } else {
            if (!(charEntered == '\b')) {
                input.push_back(charEntered);
                currentTyping.setText(
                    "INPUT:  " + input);
            }
        }
    }
}
