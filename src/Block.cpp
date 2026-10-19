// Block.cpp —— 形状数据与绘制
#include "Block.h"

#include <graphics.h>
#include <memory>

// 形状表：对照旧版 7 个子类 draw() 的布局逐状态提取
// 未用到的旋转槽位填 0（对应形状的 stateCount 保证了不会被访问）
const Point CELLS[SHAPE_COUNT][4][4] = {
    // O：2x2，不旋转
    {{{0, 0}, {1, 0}, {0, 1}, {1, 1}}, {{0, 0}, {0, 0}, {0, 0}, {0, 0}},
     {{0, 0}, {0, 0}, {0, 0}, {0, 0}}, {{0, 0}, {0, 0}, {0, 0}, {0, 0}}},
    // I：横 / 竖（竖态顶端一格在 y-1）
    {{{0, 0}, {1, 0}, {2, 0}, {3, 0}}, {{1, -1}, {1, 0}, {1, 1}, {1, 2}},
     {{0, 0}, {0, 0}, {0, 0}, {0, 0}}, {{0, 0}, {0, 0}, {0, 0}, {0, 0}}},
    // L
    {{{2, 0}, {0, 1}, {1, 1}, {2, 1}}, {{0, 0}, {1, 0}, {1, 1}, {1, 2}},
     {{0, 0}, {1, 0}, {2, 0}, {0, 1}}, {{0, 0}, {0, 1}, {0, 2}, {1, 2}}},
    // J
    {{{0, 0}, {0, 1}, {1, 1}, {2, 1}}, {{1, 0}, {1, 1}, {1, 2}, {0, 2}},
     {{0, 0}, {1, 0}, {2, 0}, {2, 1}}, {{0, 0}, {1, 0}, {0, 1}, {0, 2}}},
    // S
    {{{1, 0}, {2, 0}, {0, 1}, {1, 1}}, {{0, 0}, {0, 1}, {1, 1}, {1, 2}},
     {{0, 0}, {0, 0}, {0, 0}, {0, 0}}, {{0, 0}, {0, 0}, {0, 0}, {0, 0}}},
    // Z
    {{{0, 0}, {1, 0}, {1, 1}, {2, 1}}, {{1, 0}, {1, 1}, {0, 1}, {0, 2}},
     {{0, 0}, {0, 0}, {0, 0}, {0, 0}}, {{0, 0}, {0, 0}, {0, 0}, {0, 0}}},
    // T
    {{{1, 0}, {0, 1}, {1, 1}, {2, 1}}, {{1, 0}, {0, 1}, {1, 1}, {1, 2}},
     {{0, 1}, {1, 1}, {2, 1}, {1, 2}}, {{1, 0}, {1, 1}, {1, 2}, {2, 1}}},
};

const int STATE_COUNT[SHAPE_COUNT] = {1, 2, 4, 4, 2, 2, 4};

// 预览框内各形状的原点（单位：半格；I 偏左、O 居中，其余统一）
const int PREVIEW_X2[SHAPE_COUNT] = {24, 22, 23, 23, 23, 23, 23};

Block::Block(int shape_, int x_, int y_, int color_, int state_)
    : shape(shape_), color(color_), x(x_), y(y_), state(state_) {}

void Block::cells(Point out[4]) const {
    for (int i = 0; i < 4; ++i) out[i] = CELLS[shape][state][i];
}

void Block::draw() const {
    Point cs[4];
    cells(cs);
    setfillcolor(color);
    for (const Point& c : cs) {
        int x1 = (x + c.x) * gridSize;
        int y1 = (y + c.y) * gridSize;
        solidrectangle(x1, y1, x1 + gridSize, y1 + gridSize);
    }
}

void Block::drawPreview() const {
    Point cs[4];
    cells(cs);
    int x1 = PREVIEW_X2[shape] * halfgridSize;
    int y1 = 3 * gridSize;
    setfillcolor(color);
    for (const Point& c : cs) {
        int px = x1 + c.x * gridSize;
        int py = y1 + c.y * gridSize;
        solidrectangle(px, py, px + gridSize, py + gridSize);
    }
}

std::unique_ptr<Block> makeBlock(const BlockSpec& spec) {
    return std::make_unique<Block>(spec.shape, spec.x, spec.y, spec.color, spec.state);
}
