#include "gameSave.hpp"
#include <fstream>

#include "randomWords.hpp"

auto gameSave::saveGame(std::string const& fileName) -> void {
    auto file = std::fstream(fileName, std::ios::out | std::ios::trunc);

    file << score << "\n"
    << wordsCounter << "\n"
    << strikesCounter << "\n"
    << speed << "\n"
    << newGame;
}

auto gameSave::loadGame(std::string const& fileName) -> void {
    auto file = std::fstream(fileName);

    file >> score >> wordsCounter >> strikesCounter >> speed >> newGame;

}

auto gameSave::updateGameSave(int const& scoreNew, int const& wordsCounterNew, int const& strikesCounterNew,
        int const& speedNew, bool const& newGameNew) -> void {

    score = scoreNew;
    wordsCounter = wordsCounterNew;
    strikesCounter = strikesCounterNew;
    speed = speedNew;
    newGame = newGameNew;

}
