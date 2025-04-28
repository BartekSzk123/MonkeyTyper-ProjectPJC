#include <iostream>
#include <sfML/Graphics.hpp>
#include <string>
#include "Button.hpp"
#include "randomWords.hpp"

auto main() -> int {
    // tworzymy okienko
    auto window = sf::RenderWindow(
        sf::VideoMode({800, 600}), "Monkey",
        sf::Style::Default, sf::State::Windowed,
        sf::ContextSettings{.antiAliasingLevel = 8}
    );

    auto speed = 180;
    window.setFramerateLimit(speed);

    // tworzymy tytul
    auto currentFont = sf::Font("../Arial.ttf");
    auto Title = sf::Text(currentFont, "MonkeyTyper", 40);
    sf::FloatRect textBounds = Title.getLocalBounds();
    Title.setOrigin(sf::Vector2f(textBounds.position.x + textBounds.size.x / 2, 0));
    Title.setPosition(sf::Vector2f(window.getSize().x / 2, 0));
    Title.setFillColor(sf::Color::White);


    auto clearWindow = false;
    auto optionWindow = false;
    auto fontsPaths = std::vector<std::string>{
        "../Times New Roman.ttf",
        "../Courier.ttf",
        "../Arial.ttf"
    };
    auto fontIndex = 0;

    auto buttons = std::vector<Button *>();

    //button start
    auto startButton = Button(
        "START",
        25,
        currentFont,
        sf::Vector2f(200, 50),
        sf::Vector2f(window.getSize().x / 2, window.getSize().y / 4),
        [&]()-> void {
            clearWindow = true;
        });


    auto optionButton = Button(
        "OPTION",
        25,
        currentFont,
        sf::Vector2f(200, 50),
        sf::Vector2f(window.getSize().x / 2, window.getSize().y / 2.5),
        [&]()-> void {
            optionWindow = true;
        }
    );

    //button do zmianny czcionki
    auto fontButton = Button(
        "CHANGE FONT",
        25,
        currentFont,
        sf::Vector2f(200, 50),
        sf::Vector2f(window.getSize().x / 3, window.getSize().y / 4),
        [&]()-> void {
            window.clear(sf::Color::Black);
            currentFont = sf::Font(fontsPaths[fontIndex]);
            fontIndex = (fontIndex + 1) % fontsPaths.size();
            for (auto &button: buttons) {
                button->setFont(currentFont);
            }
        });

    //button powrotu to po klikenicu start
    auto backButton = Button(
        "BACK",
        15,
        currentFont,
        sf::Vector2f(100, 25),
        sf::Vector2f(50, 12.5),
        [&]() {
            clearWindow = false;
            optionWindow = false;
        });

    auto speedVecotor = std::vector<int>{60, 120, 180, 240};
    auto speedIndex = 0;

    auto wordsSpeedButton = Button(
        "SPEED: " + std::to_string(speed),
        25,
        currentFont,
        sf::Vector2f(200, 50),
        sf::Vector2f(window.getSize().x / 1.5, window.getSize().y / 4),
        [&]() -> void {
        });

    wordsSpeedButton.setNewFunction([&]() {
        speed = speedVecotor[speedIndex];
        speedIndex = (speedIndex + 1) % speedVecotor.size();
        window.setFramerateLimit(speed);
        wordsSpeedButton.setText("SPEED: " + std::to_string(speed));
    });

    auto charSizesVecotor = std::vector<int>{20, 25, 30, 35, 40};
    auto charSizesIndex = 0;

    auto charSizeButton = Button(
        "TEXT SIZE: " + std::to_string(randomWords::charSize),
        25,
        currentFont,
        sf::Vector2f(200, 50),
        sf::Vector2f(window.getSize().x / 2, window.getSize().y / 2.5),
        [&]()-> void {
        });

    charSizeButton.setNewFunction([&]() {
        randomWords::setRandomwordsSize(charSizesVecotor[charSizesIndex]);
        charSizesIndex = (charSizesIndex + 1) % charSizesVecotor.size();
        charSizeButton.setText("TEXT SIZE: " + std::to_string(randomWords::charSize));
    });

    auto score = 0;
    auto wordsCounter = 0;

    auto scoreBar = Button(
    "SCORE: " + std::to_string(score) + "\tTYPED WORDS: " + std::to_string(wordsCounter),
        20,
        currentFont,
        sf::Vector2f(800, 30),
        sf::Vector2f(window.getSize().x / 2, 585),
        [&]()-> void {
        });

    buttons.emplace_back(&startButton);
    buttons.emplace_back(&optionButton);
    buttons.emplace_back(&backButton);
    buttons.emplace_back(&fontButton);
    buttons.emplace_back(&wordsSpeedButton);
    buttons.emplace_back(&charSizeButton);
    buttons.emplace_back(&scoreBar);

    //event zamykania
    auto const onClose = [&window](sf::Event::Closed const &) {
        window.close();
    };

    //event klikniecia
    auto const ButtonClick =
            [&buttons]
    (sf::Event::MouseButtonPressed const &e) {
        if (e.button == sf::Mouse::Button::Left) {
            for (auto &b: buttons) {
                auto clicked = b->shape.getGlobalBounds().contains(static_cast<sf::Vector2f>(e.position));

                if (clicked) {
                    b->checkClick(sf::Vector2f(e.position.x, e.position.y));
                }
            }
        }
    };

    auto const ButtonHovered =
            [&buttons]
    (sf::Event::MouseMoved const &e) {
        for (auto &b: buttons) {
            auto hoverd = b->shape.getGlobalBounds().contains(static_cast<sf::Vector2f>(e.position));

            if (hoverd && b->isVisible) {
                b->setColor({192, 192, 192});
            } else {
                b->setColor(sf::Color::White);
            }
        }
    };

    auto generatedWords = std::vector<sf::Text>();
    auto vec = randomWords::wordsFromFile("../words.txt");
    auto input = std::string();

    auto const textEntered =
            [&](sf::Event::TextEntered const &e) {
        if (e.unicode < 128) {
            auto charEntered = static_cast<char>(e.unicode);

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
            } else if (charEntered == '\b' && !input.empty()) {
                input.pop_back();
                scoreBar.setText(
                    "SCORE: " + std::to_string(score) + "\tTYPED WORDS: " + std::to_string(wordsCounter) + input);
            } else {
                if (!(charEntered == '\b')) {
                    input.push_back(charEntered);
                    scoreBar.setText(
                        "SCORE: " + std::to_string(score) + "\tTYPED WORDS: " + std::to_string(wordsCounter) + input);
                }
            }
        }
    };

    auto clock = sf::Clock();
    while (window.isOpen()) {
        window.handleEvents(onClose, ButtonClick, ButtonHovered, textEntered);

        if (!clearWindow && !optionWindow) {
            //poczatkowe okienko
            window.clear(sf::Color::Black);
            window.draw(Title);
            startButton.setVisibility(true);
            window.draw(startButton);
            optionButton.setVisibility(true);
            window.draw(optionButton);
            fontButton.setVisibility(false);
            wordsSpeedButton.setVisibility(false);
            backButton.setVisibility(false);
            charSizeButton.setVisibility(false);
        } else if (!clearWindow && optionWindow) {
            window.clear(sf::Color::Black);
            window.draw(Title);
            startButton.setVisibility(false);
            fontButton.setVisibility(true);
            window.draw(fontButton);
            wordsSpeedButton.setVisibility(true);
            window.draw(wordsSpeedButton);
            charSizeButton.setVisibility(true);
            window.draw(charSizeButton);
            backButton.setVisibility(true);
            window.draw(backButton);
        } else {
            //okienko po startbutton
            window.clear(sf::Color::Black);
            startButton.setVisibility(false);
            optionButton.setVisibility(false);
            fontButton.setVisibility(false);
            wordsSpeedButton.setVisibility(false);
            backButton.setVisibility(true);
            window.draw(backButton);
            scoreBar.setVisibility(false);
            window.draw(scoreBar);

            if (clock.getElapsedTime().asSeconds() > 1) {
                generatedWords.emplace_back(randomWords::wordsGenerator(vec, currentFont));
                clock.restart();
            }

            for (auto &word: generatedWords) {
                word.move({1, 0});
                window.draw(word);
            }
        }

        window.display();
    }
}
