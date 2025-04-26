#pragma once
#include <SFML/Graphics.hpp>

struct randomWords {

    static int charSize;

    static auto wordsFromFile(const std::string &filePath) -> std::vector<std::string>;

    static auto wordsGenerator(const std::vector<std::string> &words,const sf::Font &chosenFont) -> sf::Text;

    static auto setRandomwordsSize(const int &size) -> void;

};