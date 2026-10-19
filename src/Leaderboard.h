// Leaderboard.h —— 排行榜（值语义容器 + 文件持久化，格式与旧版一致）
#pragma once

#include <string>
#include <vector>

#include "Player.h"

class Leaderboard {
public:
    void load(const std::string& path = "leaderboard.txt");
    void save(const std::string& path = "leaderboard.txt");
    void add(const Player& player);
    void sortByScore();

    // 按难度绘制前 10 名（唯一依赖图形库的成员）
    void show(int gameMode, int x, int y) const;

private:
    std::vector<Player> players_;
};
