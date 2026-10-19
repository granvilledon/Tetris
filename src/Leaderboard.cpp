// Leaderboard.cpp
#include "Leaderboard.h"

#include <algorithm>
#include <fstream>
#include <graphics.h>
#include <sstream>

#include "common.h"

void Leaderboard::load(const std::string& path) {
    players_.clear();
    std::ifstream file(path);
    if (!file.is_open()) return;
    std::string name;
    int score = 0, mode = 0;
    while (file >> name >> score >> mode) {
        players_.emplace_back(name, score, mode);
    }
}

void Leaderboard::save(const std::string& path) {
    std::ofstream file(path);
    if (!file.is_open()) return;
    for (const Player& p : players_) {
        file << p.getName() << ' ' << p.getScore() << ' ' << p.getGameMode() << '\n';
    }
}

void Leaderboard::add(const Player& player) {
    players_.push_back(player);
}

void Leaderboard::sortByScore() {
    std::sort(players_.begin(), players_.end(),
              [](const Player& a, const Player& b) { return a.getScore() > b.getScore(); });
}

void Leaderboard::show(int gameMode, int x, int y) const {
    int rank = 1;
    for (const Player& p : players_) {
        if (p.getGameMode() != gameMode) continue;
        std::ostringstream ss;
        ss << rank++ << ". " << p.getName() << ' ' << p.getScore();
        outtextxy(x, y, ss.str().c_str());
        y += gridSize;
        if (rank > 10) break;
    }
}
