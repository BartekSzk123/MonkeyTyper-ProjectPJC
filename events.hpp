#pragma once
#include "uiElement.hpp"
#include "gameStatus.hpp"
#include "gameSave.hpp"
#include "BestScores.hpp"
#include <SFML/Graphics.hpp>
#include <vector>
#include "SFML/Audio/Sound.hpp"


auto onClose(sf::Event::Closed const& event, sf::RenderWindow& window, gameSave &save) -> void;
auto mouseClick(sf::Event::MouseButtonPressed const& event,std::vector<uiElement*> &buttons) -> void;
auto mouseHover(sf::Event::MouseMoved const& event,std::vector<uiElement*> &buttons) -> void;
auto keyPressed(sf::Event::KeyPressed const& event, uiElement const& wordsSpeedButton, uiElement const& charSizeButton,
    uiElement const& fontButton, uiElement const& musicButton, uiElement const& soundsButton, uiElement const& wordsColorButton) -> void;
auto textEntered(sf::Event::TextEntered const &event, std::string& input,
    std::vector<sf::Text>& generatedWords, int& score, int& wordscounter,
    uiElement &scoreBar, uiElement &currentTyping, GameStatus status,
    sf::Sound &scoreSound, sf::Sound &wrongSound, bool const& soundsON,
    std::string& playerName, bool& enterName) -> void;