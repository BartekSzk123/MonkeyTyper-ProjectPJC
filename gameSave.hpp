#pragma once
#include <string>
#include <vector>
#include "SFML/System/Vector2.hpp"

struct gameSave {
    int score;
    int wordsCounter;
    int strikesCounter;
    int speed;
    bool newGame;
    std::vector<std::pair<std::string, sf::Vector2f>> saveWords;

    auto saveGame(std::string const& fileName = "../gameSave.txt") -> void;
    auto loadGame(std::string const& fileName = "../gameSave.txt") -> void;
    auto updateGameSave(int const& scoreNew, int const& wordsCounterNew, int const& strikesCounterNew,
        int const& speedNew, bool const& newGameNew) -> void;


};
