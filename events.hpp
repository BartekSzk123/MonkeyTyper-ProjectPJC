#pragma once
#include "Button.hpp"
#include "gameStatus.hpp"
#include "gameSave.hpp"
#include "BestScores.hpp"
#include <SFML/Graphics.hpp>
#include <vector>
#include "SFML/Audio/Sound.hpp"


auto onClose(sf::Event::Closed const& event, sf::RenderWindow& window, gameSave &save) -> void;
auto mouseClick(sf::Event::MouseButtonPressed const& event,std::vector<Button*> &buttons) -> void;
auto mouseHover(sf::Event::MouseMoved const& event,std::vector<Button*> &buttons) -> void;
auto keyPressed(sf::Event::KeyPressed const& event, Button const& wordsSpeedButton, Button const& charSizeButton,
    Button const& fontButton, Button const& musicButton, Button const& soundsButton) -> void;
auto textEntered(sf::Event::TextEntered const &event, std::string& input,
    std::vector<sf::Text>& generatedWords, int& score, int& wordscounter,
    Button &scoreBar, Button &currentTyping, GameStatus status,
    sf::Sound &scoreSound, sf::Sound &wrongSound, bool const& soundsON,
    std::string& playerName, bool& enterName) -> void;