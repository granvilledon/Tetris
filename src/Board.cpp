// Board.cpp —— 棋盘逻辑与绘制
#include "Board.h"

#include <algorithm>
#include <graphics.h>

#include "common.h"

Board::Board(int score, const std::string& playerName)
    : score_(score), playerName_(playerName) {
    grid_.fill({});
}

bool Board::collides(int nx, int ny, int nstate) const {
    if (!current_) return false;
    for (const Point& c : CELLS[current_->shape][nstate]) {
        int x = nx + c.x;
        int y = ny + c.y;
        if (x < 0 || x >= WIDTH || y < 0 || y >= HEIGHT) return true;  // 撞墙
        if (grid_[y][x]) return true;                                  // 撞固定块
    }
    return false;
}

bool Board::tryMove(int dx, int dy) {
    if (!current_ || collides(current_->x + dx, current_->y + dy, current_->state))
        return false;
    current_->x += dx;
    current_->y += dy;
    return true;
}

bool Board::tryRotate(bool clockwise) {
    if (!current_) return false;
    int ns = clockwise ? (current_->state + 1) % current_->stateCount()
                       : (current_->state - 1 + current_->stateCount()) % current_->stateCount();
    if (collides(current_->x, current_->y, ns)) return false;
    current_->state = ns;
    return true;
}

void Board::lockCurrent() {
    if (!current_) return;
    Point cs[4];
    current_->cells(cs);
    for (const Point& c : cs) {
        int y = current_->y + c.y;
        int x = current_->x + c.x;
        if (y >= 0 && y < HEIGHT && x >= 0 && x < WIDTH) grid_[y][x] = 1;
        // 锁在顶部缓冲区（y<0）说明已经堆出界
        if (y < 0) gameOver_ = true;
    }
    current_.reset();   // 旧版这里从不 delete，每落一块泄漏一次
    clearLines();
    if (isTopOut()) gameOver_ = true;
}

int Board::clearLines() {
    int cleared = 0;
    for (int row = HEIGHT - 1; row >= 0; --row) {
        bool full = true;
        for (int col = 0; col < WIDTH; ++col) {
            if (!grid_[row][col]) { full = false; break; }
        }
        if (!full) continue;
        ++cleared;
        for (int r = row; r > 0; --r) grid_[r] = grid_[r - 1];
        grid_[0].fill(0);
        ++row;   // 整体下移后原位重查
    }
    // 一次消 n 行得 1+2+...+n = n(n+1)/2 分（与旧版累计式结果一致）
    score_ += cleared * (cleared + 1) / 2;
    return cleared;
}

bool Board::isTopOut() const {
    for (int col = 0; col < WIDTH; ++col)
        if (grid_[0][col]) return true;
    return false;
}

bool Board::isSpawnBlocked() const {
    if (!current_) return false;
    Point cs[4];
    current_->cells(cs);
    for (const Point& c : cs) {
        int y = current_->y + c.y;
        int x = current_->x + c.x;
        if (y >= 0 && y < HEIGHT && x >= 0 && x < WIDTH && grid_[y][x]) return true;
    }
    return false;
}

int Board::drawFromBag() {
    if (bag_.empty()) {
        bag_ = {0, 1, 2, 3, 4, 5, 6};
        std::shuffle(bag_.begin(), bag_.end(), rng_);
    }
    int shape = bag_.back();
    bag_.pop_back();
    return shape;
}

void Board::spawnNext() {
    // 配色沿用旧版的形状->颜色对照
    static const int kColors[SHAPE_COUNT] = {
        BLUE, GREEN, RED, MAGENTA, DARKGRAY, LIGHTCYAN, CYAN};
    if (!next_) next_ = makeBlock({drawFromBag(), 0, WIDTH / 2 - 1, 0, 0});
    current_ = std::move(next_);
    current_->color = kColors[current_->shape];
    next_ = makeBlock({drawFromBag(), 0, WIDTH / 2 - 1, 0, 0});
    next_->color = kColors[next_->shape];
    if (isSpawnBlocked()) gameOver_ = true;   // 出生点被占：直接结束（旧版会重叠几帧才判负）
}

int Board::ghostY() const {
    if (!current_) return 0;
    int y = current_->y;
    while (!collides(current_->x, y + 1, current_->state)) ++y;
    return y;
}

void Board::stepDown() {
    if (!current_) {
        spawnNext();
        return;
    }
    if (tryMove(0, 1)) return;
    lockCurrent();                 // 落地即锁定（旧版触底缓冲的体验差异很小，去掉换确定性）
    if (!gameOver_) spawnNext();
}

void Board::draw() const {
    // 棋盘格子：占用的画深色，空的画浅色
    for (int row = 0; row < HEIGHT; ++row) {
        for (int col = 0; col < WIDTH; ++col) {
            int x1 = col * gridSize;
            int y1 = row * gridSize;
            setfillcolor(grid_[row][col] ? COLOR_BOARD : COLOR_BLOCK);
            setlinecolor(BLACK);
            fillrectangle(x1, y1, x1 + gridSize, y1 + gridSize);
        }
    }

    // 幽灵落点：只画边框，提示会落在哪里
    if (current_ && ghostY() != current_->y) {
        Point cs[4];
        current_->cells(cs);
        int gy = ghostY();
        setlinecolor(LIGHTGRAY);
        for (const Point& c : cs) {
            int x1 = (current_->x + c.x) * gridSize;
            int y1 = (gy + c.y) * gridSize;
            rectangle(x1, y1, x1 + gridSize, y1 + gridSize);
        }
    }

    if (current_) current_->draw();

    // 预览窗口
    setlinecolor(BLACK);
    rectangle(11 * gridSize, 2 * gridSize, 15 * gridSize, 6 * gridSize);
    outtextxy(11 * gridSize, gridSize, _T("Next Block"));
    if (next_) next_->drawPreview();

    // 分数与玩家名
    outtextxy(11 * gridSize, 6 * gridSize + halfgridSize,
              (_T("Score: ") + std::to_string(score_)).c_str());
    outtextxy(11 * gridSize, 8 * gridSize,
              (_T("Player: ") + playerName_).c_str());
}
