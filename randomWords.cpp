#include "randomWords.hpp"
#include <fstream>
#include <__random/random_device.h>

int randomWords::charSize = 25;
sf::Color randomWords::color = sf::Color::White;

auto randomWords::wordsFromFile(std::string const& filePath) -> std::vector<std::string> {
    auto wordsVecotr = std::vector<std::string>();
    auto file = std::fstream(filePath);
    auto word = std::string();

    while (file >> word) {
        wordsVecotr.push_back(word);
    }

    return wordsVecotr;
}

auto randomWords::wordsGenerator(std::vector<std::string> const &words, sf::Font const &chosenFont,
                                 std::vector<sf::Text> const &currentWords) -> sf::Text {
    std::random_device rd;
    std::uniform_int_distribution<int> wordDist(0, words.size() - 1);
    std::uniform_int_distribution<int> posDist(100, 499);

    for (auto i = 0; i < 100; i++) {
        auto index = wordDist(rd);
        auto word = words[index];
        auto generatedWord = sf::Text(chosenFont, word, charSize);
        generatedWord.setFillColor(color);
        auto x = 5;
        auto y = posDist(rd);
        generatedWord.setPosition(sf::Vector2f(x, y));
        auto bounds = generatedWord.getGlobalBounds();
        auto collision = false;

        for (auto const &word : currentWords) {
            if (word.getGlobalBounds().findIntersection(bounds)) {
                collision = true;
            }
        }

        if (!collision) {
            generatedWord.setPosition(sf::Vector2f(x, y));
            return generatedWord;
        }

    }

}

auto randomWords::setRandomwordsSize(int const& size) -> void {
    charSize = size;
}

auto randomWords::setRandomwordsColor(sf::Color const& newColor) -> void {
    color = newColor;
}