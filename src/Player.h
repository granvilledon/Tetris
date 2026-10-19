// Player.h —— 玩家（值语义，不再裸 new）
#pragma once

#include <string>

#include "common.h"

class Player {
public:
    Player(std::string name, int score = 0, int gameMode = MODE_EASY)
        : name_(std::move(name)), score_(score), gameMode_(gameMode) {}

    const std::string& getName() const { return name_; }
    int getScore() const { return score_; }
    int getGameMode() const { return gameMode_; }

    void setScore(int score) { score_ = score; }
    void setGameMode(int mode) { gameMode_ = mode; }

private:
    std::string name_;
    int score_;
    int gameMode_;
};
