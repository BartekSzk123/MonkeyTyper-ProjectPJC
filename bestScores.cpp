#include "BestScores.hpp"
#include <fstream>
#include <ranges>
#include <algorithm>

auto ScoresUtils::saveScore(BestScores const& bestScores, std::string const& fileName) -> void {
    auto file = std::fstream(fileName, std::fstream::out | std::fstream::app);
     file << bestScores.score << " " <<bestScores.typedWords << " " << bestScores.playerName << "\n";
}

auto ScoresUtils::loadScore(std::string const& fileName) -> std::vector<BestScores> {
    auto file = std::fstream(fileName);
    auto bestResults = std::vector<BestScores>();
    auto wordsCount = 0;
    auto score = 0;
    auto playerName = std::string();


    while (file >> score >> wordsCount >> playerName) {
        bestResults.emplace_back(BestScores(score, wordsCount,playerName));
    }

    std::ranges::sort(bestResults, [](BestScores const &a, BestScores const &b) {
        return a.score > b.score;
    });

    return bestResults;
}
