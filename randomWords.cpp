#include "randomWords.hpp"
#include <fstream>
#include <cstdlib>
#include <ctime>

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
    std::srand(std::time(0));
    auto index = std::rand() % words.size();

    auto word = words[index];
    auto x = 5;
    auto y = std::rand() % 500;

    auto generatedWord = sf::Text(chosenFont, word, 25);
    generatedWord.setFillColor(sf::Color::White);
    generatedWord.setPosition(sf::Vector2f(x, y));
    return generatedWord;
}
