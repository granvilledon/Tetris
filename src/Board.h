// Board.h —— 棋盘：纯逻辑与绘制分离
//
// 逻辑部分（碰撞/移动/旋转/锁定/消行/结束判定）不碰任何图形接口，
// 可以脱离 EasyX 单独跑单元测试；draw() 是唯一依赖图形库的成员。
#pragma once

#include <array>
#include <memory>
#include <random>
#include <string>
#include <vector>

#include "Block.h"
#include "common.h"

class Board {
public:
    static constexpr int WIDTH = 10;
    static constexpr int HEIGHT = 20;

    Board(int score, const std::string& playerName);

    // ------------------------------------------------ 纯逻辑（可单测）
    // 指定姿态放在 (nx, ny) 是否会撞墙 / 撞已固定的格子
    bool collides(int nx, int ny, int nstate) const;

    // 尝试平移，失败则原样返回 false（等价旧版"移动后碰撞回退"）
    bool tryMove(int dx, int dy);
    // 尝试旋转，被挡则状态不变（等价旧版 rotate + rerotate 回退）
    bool tryRotate(bool clockwise);

    // 自动下落一步：能走就走，走不动就锁定并生成新方块
    void stepDown();

    void lockCurrent();
    // 消行并按 1/3/6/10 计分，返回消除的行数
    int clearLines();

    bool isTopOut() const;          // 顶行已有固定块
    bool isSpawnBlocked() const;    // 新方块出生点被占
    bool gameOver() const { return gameOver_; }

    int at(int row, int col) const { return grid_[row][col]; }
    void setAt(int row, int col, int value) { grid_[row][col] = value; }

    int getScore() const { return score_; }
    const std::string& playerName() const { return playerName_; }

    Block* current() { return current_.get(); }
    Block* next() { return next_.get(); }
    const Block* current() const { return current_.get(); }
    const Block* next() const { return next_.get(); }

    void setCurrent(std::unique_ptr<Block> block) { current_ = std::move(block); }
    void setNext(std::unique_ptr<Block> block) { next_ = std::move(block); }

    // 生成方块（7-bag：把 7 种形状洗成一批依次发完，避免旧版 rand()%7 的连续干旱）
    void spawnNext();

    // 幽灵落点：当前方块一路降到底的 y
    int ghostY() const;

    int width() const { return WIDTH; }
    int height() const { return HEIGHT; }

    // ------------------------------------------------ 绘制（依赖图形库）
    void draw() const;

private:
    int drawFromBag();

    std::array<std::array<int, WIDTH>, HEIGHT> grid_{};
    std::unique_ptr<Block> current_;
    std::unique_ptr<Block> next_;
    int score_ = 0;
    std::string playerName_;
    bool gameOver_ = false;
    std::vector<int> bag_;
    std::mt19937 rng_{std::random_device{}()};
};
