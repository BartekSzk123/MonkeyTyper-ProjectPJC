#include "events.hpp"

auto onClose(sf::Event::Closed const& event, sf::RenderWindow& window, gameSave &save) -> void {
    save.saveGame();
    window.close();
}

auto mouseClick(sf::Event::MouseButtonPressed const& event,std::vector<uiElement*> &buttons) -> void {
    if (event.button == sf::Mouse::Button::Left) {
        for (auto &b: buttons) {
            auto clicked = b->shape.getGlobalBounds().contains(static_cast<sf::Vector2f>(event.position));

            if (clicked) {
                b->checkClick(sf::Vector2f(event.position.x, event.position.y));
            }
        }
    }
}

auto mouseHover(sf::Event::MouseMoved const& event,std::vector<uiElement*> &buttons) -> void {
    for (auto &b: buttons) {
        auto hovered = b->shape.getGlobalBounds().contains(static_cast<sf::Vector2f>(event.position));

        if (hovered && b->isVisible) {
            b->setShapeColor({205, 133, 63});
        } else {
            b->setShapeColor({97, 39, 24});
        }
    }
}


auto keyPressed(sf::Event::KeyPressed const& event, uiElement const& wordsSpeedButton, uiElement const& charSizeButton,
    uiElement const& fontButton, uiElement const& musicButton, uiElement const& soundsButton, uiElement const& wordsColorButton) -> void {

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S) && sf::Keyboard::isKeyPressed(sf::Keyboard::Key::LControl)) {
        wordsSpeedButton.click();
    }

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W) && sf::Keyboard::isKeyPressed(sf::Keyboard::Key::LControl)) {
        charSizeButton.click();
    }

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::F) && sf::Keyboard::isKeyPressed(sf::Keyboard::Key::LControl)) {
        fontButton.click();
    }

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::M) && sf::Keyboard::isKeyPressed(sf::Keyboard::Key::LControl)) {
        musicButton.click();
    }

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::G) && sf::Keyboard::isKeyPressed(sf::Keyboard::Key::LControl)) {
        soundsButton.click();
    }

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::C) && sf::Keyboard::isKeyPressed(sf::Keyboard::Key::LControl)) {
        wordsColorButton.click();
    }
}

auto textEntered(sf::Event::TextEntered const &event, std::string& input,
    std::vector<sf::Text>& generatedWords, int& score, int& wordsCounter,
    uiElement &scoreBar, uiElement &currentTyping, GameStatus status,
    sf::Sound &scoreSound, sf::Sound &wrongSound, bool const& soundsOn,
    std::string& playerName, bool& enterName)->void {

    if (event.unicode < 128 && status == GameStatus::GameStart) {
        auto charEntered = static_cast<char>(event.unicode);

        if (charEntered == '\n' || charEntered == '\r') {

            auto found = false;

            for (auto iterator = generatedWords.begin(); iterator != generatedWords.end(); ++iterator) {
                if (input == iterator->getString()) {

                    score += iterator->getString().getSize() * 2;
                    found = true;
                    wordsCounter++;
                    generatedWords.erase(iterator);

                    if (soundsOn) {
                        scoreSound.play();
                    }

                    scoreBar.setText(
                        "SCORE: " + std::to_string(score) + "\tTYPED WORDS: " + std::to_string(wordsCounter));
                    break;
                }
            }

            if (soundsOn) {
                if (!found) {
                    wrongSound.play();
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

    if (event.unicode < 128 && status == GameStatus::GameOver && enterName) {
        auto charEntered = static_cast<char>(event.unicode);

        if ((charEntered == '\n' || charEntered == '\r') && !playerName.empty()) {
            auto result = BestScores(score,wordsCounter,playerName);
            ScoresUtils::saveScore(result);
            enterName = false;
        }else if (charEntered == '\b' && !playerName.empty()) {
            playerName.pop_back();
        }else {
            if (!(charEntered == '\b' || charEntered == '\n' || charEntered == '\r')) {
                playerName.push_back(charEntered);
            }
        }
    }


}