#pragma once
#include <string>

#include "gameStatus.hpp"

struct gameSave {
    int score;
    int wordsCounter;
    int strikesCounter;
    int speed;
    bool newGame;

    auto saveGame(std::string const& fileName = "../gameSave.txt") -> void;
    auto loadGame(std::string const& fileName = "../gameSave.txt") -> void;
    auto updateGameSave(int const& scoreNew, int const& wordsCounterNew, int const& strikesCounterNew,
        int const& speedNew, bool const& newGameNew) -> void;


};
