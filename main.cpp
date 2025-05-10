#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <string>
#include "BestScores.hpp"
#include "Button.hpp"
#include "randomWords.hpp"
#include "gameStatus.hpp"
#include "events.hpp"
#include "SFML/Audio/Music.hpp"

auto main() -> int {
    auto window = sf::RenderWindow(
        sf::VideoMode({800, 600}), "Monkey",
        sf::Style::Titlebar | sf::Style::Close, sf::State::Windowed,
        sf::ContextSettings{.antiAliasingLevel = 8}
    );

    auto status = GameStatus::MainMenu;

    auto logoTexture =sf::Texture();
    if (!logoTexture.loadFromFile("../pngs/logo.png")) {
        return -1;
    }
    auto logo = sf::Sprite(logoTexture);
    logo.setPosition(sf::Vector2f(200, -40));

    auto currentFont = sf::Font("../Arial.ttf");
    auto gameOver = sf::Text(currentFont, "GAME OVER!!!", 65);
    auto gameOverBounds = gameOver.getLocalBounds();
    gameOver.setOrigin(sf::Vector2f(gameOverBounds.position.x + gameOverBounds.size.x / 2,
                                    gameOverBounds.position.y + gameOverBounds.size.y / 2));
    gameOver.setPosition(sf::Vector2f(window.getSize().x / 2, window.getSize().y / 4));
    gameOver.setFillColor(sf::Color::Red);

    auto fontIndex = 0;

    auto treeTexture = sf::Texture();
    if (!treeTexture.loadFromFile("../pngs/palm.png")) {
        return -1;
    }

    auto tree = sf::Sprite(treeTexture);
    tree.setPosition(sf::Vector2f(0, 40));

    auto monkeyTexture = sf::Texture();
    if (!monkeyTexture.loadFromFile("../pngs/monkey.png")) {
        return -1;
    }
    auto monkey = sf::Sprite(monkeyTexture);
    monkey.setPosition(sf::Vector2f(550, 0));

    auto monkeyKO = sf::Texture();
    if (!monkeyKO.loadFromFile("../pngs/monkeyBeng.png")) {
        return -1;
    }
    auto monkeyKOS = sf::Sprite(monkeyKO);
    monkeyKOS.setPosition(sf::Vector2f(200, 200));

    auto monkeyOp1Texture = sf::Texture();
    if (!monkeyOp1Texture.loadFromFile("../pngs/monkeyOptions1.png")) {
        return -1;
    }
    auto monkeyOp1 = sf::Sprite(monkeyOp1Texture);
    monkeyOp1.setPosition(sf::Vector2f(50, 350));

    auto monkeyOp2Texture = sf::Texture();
    if (!monkeyOp2Texture.loadFromFile("../pngs/monkeyOptions2.png")) {
        return -1;
    }
    auto monkeyOp2 = sf::Sprite(monkeyOp2Texture);
    monkeyOp2.setPosition(sf::Vector2f(550, 350));

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

    auto fontsPaths = std::vector<std::string>{
        "../Times New Roman.ttf",
        "../Courier.ttf",
        "../Arial.ttf"
    };

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
            highestResults.clear();
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
    auto speed = 120;
    window.setFramerateLimit(speed);

    auto wordsSpeedButton = Button(
        " SPEED: " + std::to_string(speed),
        25,
        currentFont,
        sf::Vector2f(200, 50),
        sf::Vector2f(window.getSize().x / 2, window.getSize().y / 1.85),
        [&]() -> void {
        });

    wordsSpeedButton.setNewFunction([&]() -> void {
        speed = speedVector[speedIndex];
        speedIndex = (speedIndex + 1) % speedVector.size();
        window.setFramerateLimit(speed);
        wordsSpeedButton.setText(" SPEED: " + std::to_string(speed));
    });

    auto charSizesVector = std::vector<int>{20, 25, 30, 35};
    auto charSizesIndex = 2;

    auto charSizeButton = Button(
        "WORDS SIZE:" + std::to_string(randomWords::charSize),
        25,
        currentFont,
        sf::Vector2f(200, 50),
        sf::Vector2f(window.getSize().x / 1.5, window.getSize().y / 4),
        [&]()-> void {
        });

    charSizeButton.setNewFunction([&]() -> void {
        randomWords::setRandomwordsSize(charSizesVector[charSizesIndex]);
        charSizesIndex = (charSizesIndex + 1) % charSizesVector.size();
        charSizeButton.setText("WORDS SIZE:" + std::to_string(randomWords::charSize));
    });

    auto music = sf::Music();
    if (!music.openFromFile("../01-theme.ogg")) {
        return -1;
    }
    music.setLooping(true);
    music.play();

    auto scoreSoundBuffer = sf::SoundBuffer();
    if (!scoreSoundBuffer.loadFromFile("../scoreSound.wav")) {
        return -1;
    }
    auto scoreSound = sf::Sound(scoreSoundBuffer);

    auto wrongSoundBuffer = sf::SoundBuffer();
    if (!wrongSoundBuffer.loadFromFile("../wrongSound.wav")) {
        return -1;
    }
    auto wrongSound = sf::Sound(wrongSoundBuffer);

    auto gameOverSoundBuffer = sf::SoundBuffer();
    if (!gameOverSoundBuffer.loadFromFile("../gameOverSound.wav")) {
        return -1;
    }
    auto gameOverSound = sf::Sound(gameOverSoundBuffer);

    auto musicOn = true;
    auto musicButton = Button(
        "MUSIC: ON",
        25,
        currentFont,
        sf::Vector2f(200, 50),
        sf::Vector2f(window.getSize().x / 3, window.getSize().y / 2.5),
        [&]() -> void {
        });

    musicButton.setNewFunction([&]() -> void {
        musicOn = !musicOn;
            if (musicOn) {
                music.play();
                musicButton.setText("MUSIC: ON");
            }else {
                music.pause();
                musicButton.setText("MUSIC: OFF");
            }
    });


    auto soundsOn = true;
    auto soundsButton = Button(
        "SOUND: ON",
        25,
        currentFont,
        sf::Vector2f(200, 50),
        sf::Vector2f(window.getSize().x / 1.5, window.getSize().y / 2.5),
        [&]() -> void {
        });

    soundsButton.setNewFunction([&]() -> void {
        soundsOn = !soundsOn;
            if (soundsOn) {
                soundsButton.setText("SOUND: ON");
            }else {
                soundsButton.setText("SOUND: OFF");
            }
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

    auto shortCutsBar = Button(
        "FONT(CTRL+F) | SPEED(CTRL+S) | WORDS SIZE(CTRL+C)\n"
        "\tON/OFF MUSIC(CTRL+M) | ON/OFF SOUND(CTRL+G)",
        20,
        currentFont,
        sf::Vector2f(800, 75),
        sf::Vector2f(400, 575),
        [&]()-> void {
        });

    buttons.emplace_back(&startButton);
    buttons.emplace_back(&optionButton);
    buttons.emplace_back(&resultsButton);
    buttons.emplace_back(&backButton);
    buttons.emplace_back(&fontButton);
    buttons.emplace_back(&wordsSpeedButton);
    buttons.emplace_back(&charSizeButton);
    buttons.emplace_back(&musicButton);
    buttons.emplace_back(&scoreBar);
    buttons.emplace_back(&currentTyping);
    buttons.emplace_back(&livesBar);
    buttons.emplace_back(&shortCutsBar);
    buttons.emplace_back(&soundsButton);

    auto generatedWords = std::vector<sf::Text>();
    auto vec = randomWords::wordsFromFile("../words.txt");
    auto input = std::string();

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
    startButton.setNewFunction([&]() -> void {
        if (newGame) {
            resetGame();
            status = GameStatus::GamePreparation;
        } else {
            status = GameStatus::GameStart;
        }
    });

    auto clock = sf::Clock();
    while (window.isOpen()) {
        window.handleEvents(

            [&](sf::Event::Closed const &event) {
                onClose(event, window);
            },

            [&](sf::Event::MouseButtonPressed const &event) {
                mouseClick(event, buttons);
            },

            [&](sf::Event::MouseMoved const &event) {
                mouseHover(event, buttons);
            },

            [&](sf::Event::TextEntered const &event) {
                textEntered(event, input, generatedWords, score, wordsCounter, scoreBar, currentTyping,
                    status, scoreSound, wrongSound, soundsOn);
            },

            [&](sf::Event::KeyPressed const &event) {
                keyPressed(event, wordsSpeedButton, charSizeButton, fontButton, musicButton, soundsButton);
            }

        );

        if (status == GameStatus::MainMenu) {
            window.clear(sf::Color::Black);
            window.draw(logo);
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
            musicButton.setVisibility(false);
            soundsButton.setVisibility(false);
            window.draw(tree);
            window.draw(monkey);
        } else if (status == GameStatus::OptionsMenu) {
            window.clear(sf::Color::Black);
            window.draw(logo);
            startButton.setVisibility(false);
            resultsButton.setVisibility(false);
            optionButton.setVisibility(false);
            fontButton.setVisibility(true);
            window.draw(fontButton);
            wordsSpeedButton.setVisibility(true);
            window.draw(wordsSpeedButton);
            charSizeButton.setVisibility(true);
            window.draw(charSizeButton);
            musicButton.setVisibility(true);
            window.draw(musicButton);
            soundsButton.setVisibility(true);
            window.draw(soundsButton);
            backButton.setVisibility(true);
            window.draw(backButton);
            shortCutsBar.setVisibility(false);
            window.draw(shortCutsBar);
            window.draw(monkeyOp1);
            window.draw(monkeyOp2);
        } else if (status == GameStatus::ResultsMenu) {
            window.clear(sf::Color::Black);
            window.draw(logo);
            startButton.setVisibility(false);
            optionButton.setVisibility(false);
            resultsButton.setVisibility(false);
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
            window.draw(logo);
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
            window.draw(logo);
            startButton.setVisibility(false);
            optionButton.setVisibility(false);
            resultsButton.setVisibility(false);
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

                        if (soundsOn) {
                            gameOverSound.play();
                        }

                        status = GameStatus::GameOver;
                    }
                }
                word.move({1, 0});
                window.draw(word);
            }
        } else if (status == GameStatus::GameOver) {
            window.clear(sf::Color::Black);
            window.draw(monkeyKOS);
            window.draw(logo);
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
    return 0;
}
