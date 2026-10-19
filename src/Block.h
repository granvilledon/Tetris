// Block.h —— 方块：一份形状表驱动所有行为
//
// 旧版这里是一个 Block 基类加 7 个子类，旋转/碰撞/固定/绘制各写一遍，
// 光是格子坐标就在 5 个碰撞函数和 fixBlock 里手抄了 5 遍（约 800 行）。
// 现在全部收敛到一张 CELLS 表：形状 = 数据，行为 = 一段代码。
#pragma once

#include <memory>

#include "common.h"

struct Point {
    int x;
    int y;
};

// CELLS[形状][旋转状态] = 该姿态下 4 个格子相对 (x, y) 的偏移
// 由原 7 个子类的 draw() 布局逐状态提取，视觉与旧版完全一致
extern const Point CELLS[SHAPE_COUNT][4][4];

// 每个形状的旋转状态数：O 不转、I/S/Z 两态、L/J/T 四态
extern const int STATE_COUNT[SHAPE_COUNT];

// 预览窗口里各形状的绘制原点（单位：半格）
extern const int PREVIEW_X2[SHAPE_COUNT];

class Block {
public:
    int shape;
    int color;
    int x;
    int y;
    int state;

    Block(int shape, int x, int y, int color, int state = 0);

    int stateCount() const { return STATE_COUNT[shape]; }

    void rotateCW() { state = (state + 1) % stateCount(); }
    void rotateCCW() { state = (state - 1 + stateCount()) % stateCount(); }
    void moveLeft() { --x; }
    void moveRight() { ++x; }
    void moveDown() { ++y; }
    void moveUp() { --y; }

    // 当前姿态的 4 个格子（输出相对偏移）
    void cells(Point out[4]) const;

    void draw() const;
    void drawPreview() const;
};

// 工厂：替代旧版 generateBlock / loadSave 里各写一遍的 7-case switch
struct BlockSpec {
    int shape, color, x, y, state;
};

std::unique_ptr<Block> makeBlock(const BlockSpec& spec);
