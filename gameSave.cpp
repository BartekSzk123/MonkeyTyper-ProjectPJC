#include "gameSave.hpp"
#include <fstream>

#include "randomWords.hpp"

auto gameSave::saveGame(std::string const& fileName) -> void {
    auto file = std::fstream(fileName, std::ios::out | std::ios::trunc);

    file << score << "\n"
    << wordsCounter << "\n"
    << strikesCounter << "\n"
    << speed << "\n"
    << newGame << "\n";

    for (auto const& word : saveWords) {
        file << word.first << " " << word.second.x << " " << word.second.y << "\n";
    }
}

auto gameSave::loadGame(std::string const& fileName) -> void {
    auto file = std::fstream(fileName);

    file >> score >> wordsCounter >> strikesCounter >> speed >> newGame;

    saveWords.clear();
    auto word = std::string();
    auto x = 0;
    auto y = 0;
    while (file >> word >> x >> y) {
        saveWords.emplace_back(word, sf::Vector2f(x, y));
    }

}

auto gameSave::updateGameSave(int const& scoreNew, int const& wordsCounterNew, int const& strikesCounterNew,
        int const& speedNew, bool const& newGameNew) -> void {

    score = scoreNew;
    wordsCounter = wordsCounterNew;
    strikesCounter = strikesCounterNew;
    speed = speedNew;
    newGame = newGameNew;

}
