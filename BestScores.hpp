#pragma once
#include <vector>

struct BestScores {

    int score;
    int typedWords;

};

namespace ScoresUtils {

    auto saveScore(BestScores const& bestScores, std::string const& fileName = "../bestScores.txt") -> void;
    auto loadScore(std::string const& fileName = "../bestScores.txt") -> std::vector<BestScores>;

}