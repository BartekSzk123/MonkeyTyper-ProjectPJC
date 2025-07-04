#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <string>
#include "BestScores.hpp"
#include "uiElement.hpp"
#include "randomWords.hpp"
#include "gameStatus.hpp"
#include "events.hpp"
#include "gameSave.hpp"
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

    auto currentFont = sf::Font("../fonts/Arial.ttf");
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

    auto uiElements = std::vector<uiElement *>();

    auto startButton = uiElement(
        "NEW GAME",
        25,
        currentFont,
        sf::Vector2f(200, 50),
        sf::Vector2f(window.getSize().x / 2, 150),
        [&]()-> void {
            status = GameStatus::GamePreparation;
        });


    auto optionButton = uiElement(
        "OPTIONS",
        25,
        currentFont,
        sf::Vector2f(200, 50),
        sf::Vector2f(window.getSize().x / 2, 300),
        [&]()-> void {
            status = GameStatus::OptionsMenu;
        }
    );

    auto fontsPaths = std::vector<std::string>{
        "../fonts/Times New Roman.ttf",
        "../fonts/Courier.ttf",
        "../fonts/Arial.ttf"
    };

    auto fontButton = uiElement(
        "CHANGE FONT",
        25,
        currentFont,
        sf::Vector2f(200, 50),
        sf::Vector2f(window.getSize().x / 3, window.getSize().y / 4),
        [&]()-> void {
            window.clear(sf::Color::Black);
            currentFont = sf::Font(fontsPaths[fontIndex]);
            fontIndex = (fontIndex + 1) % fontsPaths.size();
            for (auto &element: uiElements) {
                element->setFont(currentFont);
            }
        });

    auto backButton = uiElement(
        "BACK",
        15,
        currentFont,
        sf::Vector2f(100, 25),
        sf::Vector2f(50, 12.5),
        [&]() {
            status = GameStatus::MainMenu;
        });

    auto highestResults = std::vector<sf::Text>();
    auto resultsButton = uiElement(
        "RESULTS",
        25,
        currentFont,
        sf::Vector2f(200, 50),
        sf::Vector2f(window.getSize().x / 2, 375),
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
                                 + "  TYPED WORDS: " + std::to_string(results[i].typedWords)
                                 + "\nPLAYER: " + results[i].playerName);
                auto resultBounds = result.getLocalBounds();
                result.setOrigin(sf::Vector2f(resultBounds.size.x / 2, resultBounds.size.y / 2));
                result.setPosition(sf::Vector2f(window.getSize().x / 2, pos));

                highestResults.emplace_back(result);
                ++rank;
                pos += 75;
            }

            status = GameStatus::ResultsMenu;
        });

    auto speedVector = std::vector<std::pair<std::string, int>>{
        {"  SLOW",60},
        {"NORMAL",120},
        {"  FAST",180},
        {" CRAZY",240}};

    auto speedIndex = 1;
    auto speed = speedVector[speedIndex].second;
    window.setFramerateLimit(speed);

    auto wordsSpeedButton = uiElement(
        " SPEED: " + speedVector[speedIndex].first,
        25,
        currentFont,
        sf::Vector2f(250, 50),
        sf::Vector2f(window.getSize().x / 2, window.getSize().y / 1.85),
        [&]() -> void {
        });

    wordsSpeedButton.setNewFunction([&]() -> void {
        speedIndex = (speedIndex + 1) % speedVector.size();
        speed = speedVector[speedIndex].second;
        window.setFramerateLimit(speed);
        wordsSpeedButton.setText(" SPEED: " + speedVector[speedIndex].first);
    });

    auto charSizesVector = std::vector<int>{20, 25, 30, 35};
    auto charSizesIndex = 2;

    auto charSizeButton = uiElement(
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

    auto wordsColorVector = std::vector<sf::Color>{sf::Color::White, sf::Color::Blue, sf::Color::Yellow, sf::Color::Magenta};
    auto wordsColorIndex = 1;

    auto colorRect     = uiElement(
        "",
        25,
        currentFont,
        sf::Vector2f(250, 10),
        sf::Vector2f(window.getSize().x / 2, window.getSize().y / 1.4),
        [&]()-> void {
        });
    colorRect.setShapeColor(sf::Color::White);

    auto wordsColorButton = uiElement(
        "WORDS COLOR",
        25,
        currentFont,
        sf::Vector2f(250, 50),
        sf::Vector2f(window.getSize().x / 2, window.getSize().y / 1.5),
      [&]()-> void {
        });

    wordsColorButton.setNewFunction([&]() -> void {
        randomWords::setRandomwordsColor(wordsColorVector[wordsColorIndex]);
        colorRect.setShapeColor(wordsColorVector[wordsColorIndex]);
        wordsColorIndex = (wordsColorIndex + 1) % wordsColorVector.size();
    });

    auto music = sf::Music();
    if (!music.openFromFile("../sound/01-theme.ogg")) {
        return -1;
    }
    music.setLooping(true);
    music.play();

    auto scoreSoundBuffer = sf::SoundBuffer();
    if (!scoreSoundBuffer.loadFromFile("../sound/scoreSound.wav")) {
        return -1;
    }
    auto scoreSound = sf::Sound(scoreSoundBuffer);

    auto wrongSoundBuffer = sf::SoundBuffer();
    if (!wrongSoundBuffer.loadFromFile("../sound/wrongSound.wav")) {
        return -1;
    }
    auto wrongSound = sf::Sound(wrongSoundBuffer);

    auto gameOverSoundBuffer = sf::SoundBuffer();
    if (!gameOverSoundBuffer.loadFromFile("../sound/gameOverSound.wav")) {
        return -1;
    }
    auto gameOverSound = sf::Sound(gameOverSoundBuffer);

    auto musicOn = true;
    auto musicButton = uiElement(
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
    auto soundsButton = uiElement(
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

    auto scoreBar = uiElement(
        "SCORE: " + std::to_string(score) + "\tTYPED WORDS: " + std::to_string(wordsCounter),
        20,
        currentFont,
        sf::Vector2f(600, 30),
        sf::Vector2f(500, 585),
        [&]()-> void {
        });

    auto currentTyping = uiElement(
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

    auto livesBar = uiElement(
        " STRIKES",
        25,
        currentFont,
        sf::Vector2f(110, 25),
        sf::Vector2f(745, 12.5),
        [&]()-> void {
        });
    livesBar.setTextColor(sf::Color::Red);

    auto shortCutsBar = uiElement(
        "FONT(CTRL+F) | SPEED(CTRL+S) | SIZE(CTRL+W) | COLOR(CTRL+C)\n"
        "\t\tON/OFF MUSIC(CTRL+M) | ON/OFF SOUND(CTRL+G)",
        20,
        currentFont,
        sf::Vector2f(800, 75),
        sf::Vector2f(400, 575),
        [&]()-> void {
        });


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
    auto gamePause = false;
    auto save = gameSave();
    save.loadGame();
    newGame = save.newGame;
    auto noGameToResume = false;

    auto resumeGameButton = uiElement(
        "RESUME",
        25,
        currentFont,
        sf::Vector2f(200, 50),
        sf::Vector2f(window.getSize().x / 2, 225),
        [&]()-> void {

            if (gamePause) {
                noGameToResume = false;
                gamePause = false;
                status = GameStatus::GameStart;

                scoreBar.setText(
                    "SCORE: " + std::to_string(score) + "\tTYPED WORDS: " + std::to_string(wordsCounter));

                for (auto i = 0; i < strikesCounter; i++) {
                    strikes += "X";
                }

                livesBar.setText("  " + strikes);
                return;
            }

            if (!newGame) {
                noGameToResume = false;
                score = save.score;
                wordsCounter = save.wordsCounter;
                strikesCounter = save.strikesCounter;
                speed = save.speed;
                window.setFramerateLimit(speed);

                generatedWords.clear();
                for (auto const& savedWord : save.saveWords) {
                    auto word = sf::Text(currentFont, savedWord.first ,randomWords::charSize);
                    word.setFillColor(randomWords::color);
                    word.setPosition(savedWord.second);
                    generatedWords.push_back(word);
                }


                scoreBar.setText(
                    "SCORE: " + std::to_string(score) + "\tTYPED WORDS: " + std::to_string(wordsCounter));

                for (auto i = 0; i < strikesCounter; i++) {
                    strikes += "X";
                }

                livesBar.setText("  " + strikes);


                status = GameStatus::GameStart;
            }else {
                noGameToResume = true;
            }
        });

    startButton.setNewFunction([&]() -> void {
        resetGame();
        save.updateGameSave(score, wordsCounter, strikesCounter, speed, newGame);
        status = GameStatus::GamePreparation;
    });

    backButton.setNewFunction([&]() -> void {
        save.updateGameSave(score, wordsCounter, strikesCounter, speed, newGame);
        gamePause = (status == GameStatus::GameStart);
        strikes = std::string();
        status = GameStatus::MainMenu;
    });

    auto playerName = std::string();
    auto enterName = false;
    auto nameInputTex = sf::Text(currentFont, "PLAYER NAME: ", 25);
    nameInputTex.setFillColor(sf::Color::White);
    nameInputTex.setPosition(sf::Vector2f(250, 500));

    auto noResumeText = sf::Text(currentFont, " THERE IS NO GAME TO RESUME", 25);
    noResumeText.setFillColor(sf::Color::Red);
    auto noResumeBounds = noResumeText.getLocalBounds();
    noResumeText.setOrigin(sf::Vector2f(noResumeBounds.position.x  + noResumeBounds.size.x/ 2,
                                        noResumeBounds.position.y + noResumeBounds.size.y/ 2));
    noResumeText.setPosition(sf::Vector2f(window.getSize().x / 2, 450));


    uiElements.emplace_back(&startButton);
    uiElements.emplace_back(&optionButton);
    uiElements.emplace_back(&resultsButton);
    uiElements.emplace_back(&backButton);
    uiElements.emplace_back(&fontButton);
    uiElements.emplace_back(&wordsSpeedButton);
    uiElements.emplace_back(&charSizeButton);
    uiElements.emplace_back(&wordsColorButton);
    uiElements.emplace_back(&musicButton);
    uiElements.emplace_back(&resumeGameButton);
    uiElements.emplace_back(&scoreBar);
    uiElements.emplace_back(&currentTyping);
    uiElements.emplace_back(&livesBar);
    uiElements.emplace_back(&shortCutsBar);
    uiElements.emplace_back(&soundsButton);


    auto clock = sf::Clock();
    while (window.isOpen()) {
        window.handleEvents(

            [&](sf::Event::Closed const &event) {

                save.saveWords.clear();
                for (auto const& word : generatedWords) {
                    save.saveWords.emplace_back(word.getString(), word.getPosition());
                }

                save.updateGameSave(score, wordsCounter, strikesCounter, speed, newGame);
                onClose(event, window,save);
            },

            [&](sf::Event::MouseButtonPressed const &event) {
                mouseClick(event, uiElements);
            },

            [&](sf::Event::MouseMoved const &event) {
                mouseHover(event, uiElements);
            },

            [&](sf::Event::TextEntered const &event) {
                textEntered(event, input, generatedWords, score, wordsCounter, scoreBar, currentTyping,
                    status, scoreSound, wrongSound, soundsOn,playerName, enterName);
            },

            [&](sf::Event::KeyPressed const &event) {
                keyPressed(event, wordsSpeedButton, charSizeButton, fontButton, musicButton, soundsButton,wordsColorButton);
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
            resumeGameButton.setVisibility(true);
            window.draw(resumeGameButton);
            fontButton.setVisibility(false);
            wordsSpeedButton.setVisibility(false);
            wordsColorButton.setVisibility(false);
            colorRect.setVisibility(false);
            backButton.setVisibility(false);
            charSizeButton.setVisibility(false);
            musicButton.setVisibility(false);
            soundsButton.setVisibility(false);
            window.draw(tree);
            window.draw(monkey);

            if (noGameToResume) {
                window.draw(noResumeText);
            }

            if (clock.getElapsedTime().asSeconds() > 2) {
                noGameToResume = false;
                clock.restart();
            }

        } else if (status == GameStatus::OptionsMenu) {
            window.clear(sf::Color::Black);
            window.draw(logo);
            startButton.setVisibility(false);
            resumeGameButton.setVisibility(false);
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
            wordsColorButton.setVisibility(true);
            window.draw(wordsColorButton);
            window.draw(colorRect);
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
            resumeGameButton.setVisibility(false);
            backButton.setVisibility(true);
            window.draw(backButton);

            if (!highestResults.empty()) {
                for (auto const& result: highestResults) {
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
            resumeGameButton.setVisibility(false);
            backButton.setVisibility(true);
            window.draw(backButton);
            scoreBar.setVisibility(false);
            window.draw(scoreBar);
            currentTyping.setVisibility(false);
            window.draw(currentTyping);
            livesBar.setVisibility(false);
            window.draw(livesBar);

            if (clock.getElapsedTime().asSeconds() > 1.1) {
                generatedWords.emplace_back(randomWords::wordsGenerator(vec, currentFont,generatedWords));
                clock.restart();
            }

            for (auto &word: generatedWords) {
                auto bounds = word.getGlobalBounds();
                if (bounds.position.x + bounds.size.x == 800) {
                    strikesCounter++;
                    strikes += "X";

                    livesBar.setText("  " + strikes);
                    if (strikesCounter >= 5) {

                        playerName.clear();
                        enterName = true;

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

            if (enterName) {
                nameInputTex.setString("PLAYER NAME: " + playerName);
                window.draw(nameInputTex);
            }

            newGame = true;
        }
        window.display();
    }
    return 0;
}