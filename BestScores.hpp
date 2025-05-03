#pragma once
#include <vector>

struct BestScores {

    int score;
    int typedWords;

};

namespace ScoresUtils {

    auto saveScore(const BestScores &bestScores, const std::string &fileName = "../bestScores.txt") -> void;
    auto loadScore(const std::string &fileName = "../bestScores.txt") -> std::vector<BestScores>;

}