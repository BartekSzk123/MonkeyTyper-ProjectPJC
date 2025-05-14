#pragma once
#include <SFML/Graphics.hpp>

struct randomWords {
    static int charSize;

    static auto wordsFromFile(std::string const& filePath) -> std::vector<std::string>;

    static auto wordsGenerator(std::vector<std::string> const& words, sf::Font const& chosenFont) -> sf::Text;

    static auto setRandomwordsSize(int const& size) -> void;
};
