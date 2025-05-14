#include "BestScores.hpp"
#include <fstream>
#include <ranges>
#include <algorithm>

auto ScoresUtils::saveScore(BestScores const& bestScores, std::string const& fileName) -> void {
    auto file = std::fstream(fileName, std::fstream::out | std::fstream::app);
     file << bestScores.score << " " <<bestScores.typedWords << "\n";
}

auto ScoresUtils::loadScore(std::string const& fileName) -> std::vector<BestScores> {
     auto file = std::fstream(fileName);
     auto bestResults = std::vector<BestScores>();
     auto wordsCount =0;
     auto score = 0;

     while (file >> score >> wordsCount) {
        bestResults.emplace_back(BestScores(score, wordsCount));
     }

     std::ranges::sort(bestResults, [](BestScores const& a, BestScores const& b){
        return a.score > b.score;
     });

     return bestResults;
}
