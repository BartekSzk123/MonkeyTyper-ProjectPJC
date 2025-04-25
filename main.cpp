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

    // tworzymy tytul
    auto currentFont = sf::Font("../Arial.ttf");
    auto Title = sf::Text(currentFont, "MonkeyTyper", 40);
    sf::FloatRect textBounds = Title.getLocalBounds();
    Title.setOrigin(sf::Vector2f(textBounds.position.x + textBounds.size.x / 2.0f, 0));
    Title.setPosition(sf::Vector2f(window.getSize().x / 2, 0));
    Title.setFillColor(sf::Color::White);


    auto clearWindow = false;
    auto newWindow = false;
    auto fontsPaths = std::vector<std::string>{
        "../Times New Roman.ttf",
        "../Courier.ttf",
        "../Arial.ttf"
    };
    auto fontIndex = 0;

    auto buttons = std::vector<Button*>();

    //button start
    auto startButton = Button(
        "START",
        currentFont,
        sf::Vector2f(window.getSize().x / 2, window.getSize().y / 4),
        [&]() {
            clearWindow = true;
        });

    //button do zmianny czcionki
    auto fontButton = Button(
        "CHANGE FONT",
        currentFont,
        sf::Vector2f(window.getSize().x / 2.0f, window.getSize().y / 2.5f),
        [&buttons, &newWindow,&currentFont, &fontIndex, &fontsPaths]() {
            newWindow = true;
            currentFont = sf::Font(fontsPaths[fontIndex]);
            fontIndex = (fontIndex + 1) % fontsPaths.size();
            for (auto &button : buttons) {
                button->setFont(currentFont);
            }
        });


    //button powrotu to po klikenicu start
    auto backButton = Button(
       "BACK",
       currentFont,
       sf::Vector2f(100, 25),
       [&]() {
           clearWindow = false;
       });

    buttons.emplace_back(&startButton);
    buttons.emplace_back(&backButton);
    buttons.emplace_back(&fontButton);

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
                        b->checkClick(sf::Vector2f(e.position.x,e.position.y));
                    }
                }
            }
    };

    auto const ButtonHovered =
            [&buttons]
            (sf::Event::MouseMoved const &e) {
            for (auto &b: buttons) {
                auto hoverd = b->shape.getGlobalBounds().contains(static_cast<sf::Vector2f>(e.position));

                if (hoverd) {
                    b->setColor({192,192,192});
                }else {
                    b->setColor(sf::Color::White);
                }
            }
    };

    while (window.isOpen()) {

        window.handleEvents(onClose, ButtonClick, ButtonHovered);

        if (!clearWindow && !newWindow) { //poczatkowe okienko
            window.clear(sf::Color::Black);
            window.draw(Title);
            startButton.setVisibility(true);
            window.draw(startButton);
            fontButton.setVisibility(true);
            window.draw(fontButton);

        }else if (!clearWindow && newWindow){ //okienko po zmiane czcionki
            window.clear(sf::Color::Black);
            window.draw(Title);
            startButton.setVisibility(true);
            window.draw(startButton);
            fontButton.setVisibility(true);
            window.draw(fontButton);
        }else { //okienko po startbutton
            window.clear(sf::Color::Black);
            startButton.setVisibility(false);
            fontButton.setVisibility(false);
            window.draw(backButton);

            static auto vec = randomWords::wordsFromFile("/Users/bartekszkola/Documents/PJC/MONKEY3/words.txt");
            auto randomText = randomWords::wordsGenerator(vec, currentFont);
            window.draw(randomText);

        }

        window.display();

    }
}
