#include "randomWords.hpp"
#include <fstream>
#include <__random/random_device.h>

int randomWords::charSize = 25;

auto randomWords::wordsFromFile(const std::string &filePath) -> std::vector<std::string> {
    auto wordsVecotr = std::vector<std::string>();
    auto file = std::fstream(filePath);
    auto word = std::string();

    while (file >> word) {
        wordsVecotr.push_back(word);
    }

    return wordsVecotr;
}

auto randomWords::wordsGenerator(const std::vector<std::string> &words, const sf::Font &chosenFont) -> sf::Text {
    std::random_device rd;
    std::uniform_int_distribution<int> wordDist(0, words.size() - 1);
    std::uniform_int_distribution<int> posDist(100, 499);

    auto index = wordDist(rd);

    auto word = words[index];
    auto x = 5;
    auto y = posDist(rd);

    auto generatedWord = sf::Text(chosenFont, word, charSize);
    generatedWord.setFillColor(sf::Color::White);
    generatedWord.setPosition(sf::Vector2f(x, y));
    return generatedWord;
}

auto randomWords::setRandomwordsSize(const int &size) -> void {
    charSize = size;
}
