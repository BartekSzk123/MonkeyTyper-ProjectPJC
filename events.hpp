#pragma once
#include "Button.hpp"
#include "gameStatus.hpp"
#include <SFML/Graphics.hpp>
#include <vector>


auto onClose(sf::Event::Closed const& event, sf::RenderWindow& window) -> void;
auto mouseClick(sf::Event::MouseButtonPressed const& event,std::vector<Button*> &buttons) -> void;
auto mouseHover(sf::Event::MouseMoved const& event,std::vector<Button*> &buttons) -> void;
auto keyPressed(sf::Event::KeyPressed const& event, Button const& wordsSpeedButton, Button const& charSizeButton,
    Button const& fontButton, Button const& musicButton) -> void;
auto textEntered(sf::Event::TextEntered const &event, std::string& input,
    std::vector<sf::Text>& generatedWords, int& score, int& wordscounter,
    Button &scoreBar, Button &currentTyping, GameStatus status) -> void;
