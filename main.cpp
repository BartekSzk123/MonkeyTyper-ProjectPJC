#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <string>
#include "BestScores.hpp"
#include "Button.hpp"
#include "randomWords.hpp"
#include "gameStatus.hpp"
#include "SFML/Audio/Music.hpp"

auto main() -> int {
    auto window = sf::RenderWindow(
        sf::VideoMode({800, 600}), "Monkey",
        sf::Style::Titlebar | sf::Style::Close, sf::State::Windowed,
        sf::ContextSettings{.antiAliasingLevel = 8}
    );

    auto status = GameStatus::MainMenu;
    auto speed = 120;
    window.setFramerateLimit(speed);

    auto currentFont = sf::Font("../Arial.ttf");
    auto Title = sf::Text(currentFont, "MonkeyTyper", 40);
    auto titleBounds = Title.getLocalBounds();
    Title.setOrigin(sf::Vector2f(titleBounds.position.x + titleBounds.size.x / 2, 0));
    Title.setPosition(sf::Vector2f(window.getSize().x / 2, 0));
    Title.setFillColor(sf::Color::Yellow);

    auto gameOver = sf::Text(currentFont, "GAME OVER!!!", 65);
    auto gameOverBounds = gameOver.getLocalBounds();
    gameOver.setOrigin(sf::Vector2f(gameOverBounds.position.x + gameOverBounds.size.x / 2,
                                    gameOverBounds.position.y + gameOverBounds.size.y / 2));
    gameOver.setPosition(sf::Vector2f(window.getSize().x / 2, window.getSize().y / 4));
    gameOver.setFillColor(sf::Color::Red);

    auto fontsPaths = std::vector<std::string>{
        "../Times New Roman.ttf",
        "../Courier.ttf",
        "../Arial.ttf"
    };
    auto fontIndex = 0;

    auto treeTexture = sf::Texture();
    if (!treeTexture.loadFromFile("../tree.png")) {
        return -1;
    }

    auto tree = sf::Sprite(treeTexture);
    tree.setPosition(sf::Vector2f(0, 40));

    auto monkeyTexture = sf::Texture();
    if (!monkeyTexture.loadFromFile("../monke.png")) {
        return -1;
    }
    auto monkey = sf::Sprite(monkeyTexture);
    monkey.setPosition(sf::Vector2f(550, 0));

    auto monkeyKO = sf::Texture();
    if (!monkeyKO.loadFromFile("../monkeKO.png")) {
        return -1;
    }

    auto monkeyKOS = sf::Sprite(monkeyKO);
    monkeyKOS.setPosition(sf::Vector2f(200, 200));

    auto buttons = std::vector<Button *>();

    auto startButton = Button(
        "START",
        25,
        currentFont,
        sf::Vector2f(200, 50),
        sf::Vector2f(window.getSize().x / 2, window.getSize().y / 4),
        [&]()-> void {
            status = GameStatus::GamePreparation;
        });


    auto optionButton = Button(
        "OPTIONS",
        25,
        currentFont,
        sf::Vector2f(200, 50),
        sf::Vector2f(window.getSize().x / 2, window.getSize().y / 2.5),
        [&]()-> void {
            status = GameStatus::OptionsMenu;
        }
    );

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

    auto backButton = Button(
        "BACK",
        15,
        currentFont,
        sf::Vector2f(100, 25),
        sf::Vector2f(50, 12.5),
        [&]() {
            status = GameStatus::MainMenu;
        });


    auto highestResults = std::vector<sf::Text>();
    auto resultsButton = Button(
        "RESULTS",
        25,
        currentFont,
        sf::Vector2f(200, 50),
        sf::Vector2f(window.getSize().x / 2, window.getSize().y / 1.9),
        [&]() {
            auto results = ScoresUtils::loadScore();
            auto rank = 1;
            auto pos = 150;

            if (results.empty()) {
                status = GameStatus::ResultsMenu;
                return;
            }

            for (auto i = 0; i < 5; ++i) {
                auto result = sf::Text(currentFont, " ", 15);
                result.setFillColor(sf::Color::White);
                result.setString(std::to_string(rank) + ". SCORE:  " + std::to_string(results[i].score)
                                 + "  TYPED WORDS: " + std::to_string(results[i].typedWords));
                auto resultBounds = result.getLocalBounds();
                result.setOrigin(sf::Vector2f(resultBounds.size.x / 2, resultBounds.size.y / 2));
                result.setPosition(sf::Vector2f(window.getSize().x / 2, pos));

                highestResults.emplace_back(result);
                ++rank;
                pos += 50;
            }

            status = GameStatus::ResultsMenu;
        });

    auto speedVector = std::vector<int>{60, 120, 180, 240};
    auto speedIndex = 0;

    auto wordsSpeedButton = Button(
        "SPEED: " + std::to_string(speed),
        25,
        currentFont,
        sf::Vector2f(200, 50),
        sf::Vector2f(window.getSize().x / 1.5, window.getSize().y / 4),
        [&]() -> void {
        });

    wordsSpeedButton.setNewFunction([&]() -> void {
        speed = speedVector[speedIndex];
        speedIndex = (speedIndex + 1) % speedVector.size();
        window.setFramerateLimit(speed);
        wordsSpeedButton.setText("SPEED: " + std::to_string(speed));
    });

    auto charSizesVector = std::vector<int>{20, 25, 30, 35, 40};
    auto charSizesIndex = 2;

    auto charSizeButton = Button(
        "TEXT SIZE: " + std::to_string(randomWords::charSize),
        25,
        currentFont,
        sf::Vector2f(200, 50),
        sf::Vector2f(window.getSize().x / 2, window.getSize().y / 2.5),
        [&]()-> void {
        });

    charSizeButton.setNewFunction([&]() -> void {
        randomWords::setRandomwordsSize(charSizesVector[charSizesIndex]);
        charSizesIndex = (charSizesIndex + 1) % charSizesVector.size();
        charSizeButton.setText("TEXT SIZE: " + std::to_string(randomWords::charSize));
    });

    auto score = 0;
    auto wordsCounter = 0;

    auto scoreBar = Button(
        "SCORE: " + std::to_string(score) + "\tTYPED WORDS: " + std::to_string(wordsCounter),
        20,
        currentFont,
        sf::Vector2f(600, 30),
        sf::Vector2f(500, 585),
        [&]()-> void {
        });

    auto currentTyping = Button(
        "INPUT:  ",
        20,
        currentFont,
        sf::Vector2f(200, 30),
        sf::Vector2f(100, 585),
        [&]()-> void {
        });
    currentTyping.setTextColor(sf::Color::Green);

    auto strikesCounter = 0;
    auto strikes = std::string();

    auto livesBar = Button(
        " STRIKES",
        25,
        currentFont,
        sf::Vector2f(110, 25),
        sf::Vector2f(745, 12.5),
        [&]()-> void {
        });
    livesBar.setTextColor(sf::Color::Red);

    buttons.emplace_back(&startButton);
    buttons.emplace_back(&optionButton);
    buttons.emplace_back(&resultsButton);
    buttons.emplace_back(&backButton);
    buttons.emplace_back(&fontButton);
    buttons.emplace_back(&wordsSpeedButton);
    buttons.emplace_back(&charSizeButton);
    buttons.emplace_back(&scoreBar);
    buttons.emplace_back(&currentTyping);
    buttons.emplace_back(&livesBar);

    auto const onClose = [&window](sf::Event::Closed const &) {
        window.close();
    };

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
            auto hovered = b->shape.getGlobalBounds().contains(static_cast<sf::Vector2f>(e.position));

            if (hovered && b->isVisible) {
                b->setShapeColor({205, 133, 63});
            } else {
                b->setShapeColor({97, 39, 24});
            }
        }
    };

    auto generatedWords = std::vector<sf::Text>();
    auto vec = randomWords::wordsFromFile("../words.txt");
    auto input = std::string();

    auto const textEntered =
            [&](sf::Event::TextEntered const &e) {
        if (e.unicode < 128 && status == GameStatus::GameStart) {
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
    };

    auto gamePrepText = sf::Text(currentFont, "3", 100);
    auto gamePrepBounds = gamePrepText.getLocalBounds();
    gamePrepText.setOrigin(sf::Vector2f(gamePrepBounds.position.x + gamePrepBounds.size.x / 2,
                                        gamePrepBounds.position.y + gamePrepBounds.size.y / 2));
    gamePrepText.setPosition(sf::Vector2f(window.getSize().x / 2, window.getSize().y / 2));
    gamePrepText.setFillColor(sf::Color::Yellow);
    auto gamePrepCounter = 3;

    auto resetGame = [&]() {
        score = 0;
        wordsCounter = 0;
        strikesCounter = 0;
        gamePrepCounter = 3;
        generatedWords.clear();
        input.clear();
        strikes.clear();
        livesBar.setText(" STRIKES");
        currentTyping.setText("INPUT:  ");
        gamePrepText.setString("3");
        scoreBar.setText(
        "SCORE: " + std::to_string(score) + "\tTYPED WORDS: " + std::to_string(wordsCounter));

    };

    auto newGame = true;

    startButton.setNewFunction([&]() -> void  {
        if (newGame) {
            resetGame();
            status = GameStatus::GamePreparation;
        }else {
            status = GameStatus::GameStart;
        }
    });

    auto music = sf::Music();
    if (!music.openFromFile("../01-theme.ogg")) {
        return -1;
    }
    music.setLooping(true);
    music.play();

    auto clock = sf::Clock();
    while (window.isOpen()) {
        window.handleEvents(onClose, ButtonClick, ButtonHovered, textEntered);

        if (status == GameStatus::MainMenu) {
            window.clear(sf::Color::Black);
            window.draw(Title);
            startButton.setVisibility(true);
            window.draw(startButton);
            optionButton.setVisibility(true);
            window.draw(optionButton);
            resultsButton.setVisibility(true);
            window.draw(resultsButton);
            fontButton.setVisibility(false);
            wordsSpeedButton.setVisibility(false);
            backButton.setVisibility(false);
            charSizeButton.setVisibility(false);
            window.draw(tree);
            window.draw(monkey);
        } else if (status == GameStatus::OptionsMenu) {
            window.clear(sf::Color::Black);
            window.draw(Title);
            startButton.setVisibility(false);
            resultsButton.setVisibility(false);
            fontButton.setVisibility(true);
            window.draw(fontButton);
            wordsSpeedButton.setVisibility(true);
            window.draw(wordsSpeedButton);
            charSizeButton.setVisibility(true);
            window.draw(charSizeButton);
            backButton.setVisibility(true);
            window.draw(backButton);
        } else if (status == GameStatus::ResultsMenu) {
            window.clear(sf::Color::Black);
            window.draw(Title);
            startButton.setVisibility(false);
            optionButton.setVisibility(false);
            resultsButton.setVisibility(false);
            fontButton.setVisibility(false);
            wordsSpeedButton.setVisibility(false);
            backButton.setVisibility(true);
            window.draw(backButton);

            if (!highestResults.empty()) {
                for (auto result: highestResults) {
                    window.draw(result);
                }
            }

            window.draw(tree);
            window.draw(monkey);
        } else if (status == GameStatus::GamePreparation) {
            window.clear(sf::Color::Black);
            window.draw(Title);
            if (clock.getElapsedTime().asSeconds() > 1) {
                clock.restart();
                --gamePrepCounter;
                gamePrepText.setString(std::to_string(gamePrepCounter));

                if (gamePrepCounter == 0) {
                    status = GameStatus::GameStart;
                }
            }
            window.draw(gamePrepText);
        } else if (status == GameStatus::GameStart) {
            newGame = false;
            window.clear(sf::Color::Black);
            window.draw(Title);
            startButton.setVisibility(false);
            optionButton.setVisibility(false);
            resultsButton.setVisibility(false);
            fontButton.setVisibility(false);
            wordsSpeedButton.setVisibility(false);
            backButton.setVisibility(true);
            window.draw(backButton);
            scoreBar.setVisibility(false);
            window.draw(scoreBar);
            currentTyping.setVisibility(false);
            window.draw(currentTyping);
            livesBar.setVisibility(false);
            window.draw(livesBar);

            if (clock.getElapsedTime().asSeconds() > 1) {
                generatedWords.emplace_back(randomWords::wordsGenerator(vec, currentFont));
                clock.restart();
            }

            for (auto &word: generatedWords) {
                auto bounds = word.getGlobalBounds();
                if (bounds.position.x + bounds.size.x == 800) {
                    strikesCounter++;
                    strikes += "X";

                    livesBar.setText("  " + strikes);
                    if (strikesCounter >= 5) {
                        auto result = BestScores(score, wordsCounter);
                        ScoresUtils::saveScore(result);
                        status = GameStatus::GameOver;
                    }
                }
                word.move({1, 0});
                window.draw(word);
            }
        } else if (status == GameStatus::GameOver) {
            window.clear(sf::Color::Black);
            window.draw(monkeyKOS);
            window.draw(Title);
            window.draw(gameOver);
            backButton.setVisibility(true);
            window.draw(backButton);
            window.draw(scoreBar);
            window.draw(currentTyping);
            window.draw(livesBar);
            newGame = true;
        }
        window.display();
    }
}