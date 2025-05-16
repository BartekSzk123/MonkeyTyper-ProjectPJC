#pragma once
#include <SFML/Graphics.hpp>

struct randomWords {
    static int charSize;
    static sf::Color color;

    static auto wordsFromFile(std::string const& filePath) -> std::vector<std::string>;

    static auto wordsGenerator(std::vector<std::string> const& words, sf::Font const& chosenFont, std::vector<sf::Text> const& currentWords) -> sf::Text;

    static auto setRandomwordsSize(int const& size) -> void;

    static auto setRandomwordsColor(sf::Color const& newColor) -> void;
};
