// test_board.cpp —— 棋盘纯逻辑单元测试（不依赖图形库，链接 graphics_stub）
//
// 覆盖旧版翻过车的几个点：边界、越界旋转、消行计分、结束判定、贴墙移动。
#include <cassert>
#include <cstdio>
#include <string>

#include "Board.h"

namespace {

int g_failed = 0;

#define CHECK(cond)                                                        \
    do {                                                                   \
        if (cond) {                                                        \
            std::printf("  [PASS] %s\n", #cond);                           \
        } else {                                                           \
            std::printf("  [FAIL] %s  (%s:%d)\n", #cond, __FILE__, __LINE__); \
            ++g_failed;                                                    \
        }                                                                  \
    } while (0)

Board makeBoard() {
    Board b(0, "test");
    // 造一个 I 块（横放，4 格）放在出生点，便于测试移动与锁定
    b.setNext(std::make_unique<Block>(SHAPE_I, Board::WIDTH / 2 - 1, 0, GREEN, 0));
    b.spawnNext();
    return b;
}

void testBounds() {
    std::printf("== 边界：贴墙不能再移 ==\n");
    Board b = makeBoard();
    // I 横放 4 格：出生 x=4，可右移 2 步到 x=6（右缘 x=9）
    CHECK(b.tryMove(1, 0));
    CHECK(b.tryMove(1, 0));
    CHECK(!b.tryMove(1, 0));          // 再往右就越界，必须被拒绝
    // 从 x=6 左移 6 步到 x=0，第 7 步越界
    CHECK(b.tryMove(-1, 0));
    CHECK(b.tryMove(-1, 0));
    CHECK(b.tryMove(-1, 0));
    CHECK(b.tryMove(-1, 0));
    CHECK(b.tryMove(-1, 0));
    CHECK(b.tryMove(-1, 0));
    CHECK(!b.tryMove(-1, 0));
    std::printf("\n");
}

void testRotateAtTop() {
    std::printf("== 旋转：I 在顶部不能转成竖直（会越出棋盘上沿） ==\n");
    Board b = makeBoard();
    int stateBefore = b.current()->state;
    CHECK(!b.tryRotate(true));        // 竖直态的顶端格子在 y=-1，必须拒绝
    CHECK(b.current()->state == stateBefore);
    // 下移一格后就能转了
    CHECK(b.tryMove(0, 1));
    CHECK(b.tryRotate(true));
    std::printf("\n");
}

void testCollisionWithStack() {
    std::printf("== 碰撞：不能移进已固定的格子 ==\n");
    Board b = makeBoard();
    b.setAt(0, 9, 1);                 // 右上角固定块
    CHECK(b.tryMove(1, 0));           // x=5：格子 5..8，不碰
    CHECK(!b.tryMove(1, 0));          // x=6：格子 6..9 撞上它，拒绝
    CHECK(b.tryMove(-1, 0));
    CHECK(b.tryMove(-1, 0));
    CHECK(b.tryMove(-1, 0));
    CHECK(b.tryMove(-1, 0));
    CHECK(b.tryMove(-1, 0));          // 一路左移到 x=0
    CHECK(!b.tryMove(-1, 0));         // 左端越界
    std::printf("\n");
}

void presetExceptCol0(Board& b, int row) {  // 除第 0 列外整行预填
    for (int c = 1; c < Board::WIDTH; ++c) b.setAt(row, c, 1);
}

void testClearOneLine() {
    std::printf("== 消 1 行 = 1 分 ==\n");
    Board b(0, "test");
    presetExceptCol0(b, 19);
    b.setNext(std::make_unique<Block>(SHAPE_I, 0, 19, GREEN, 0));   // 补上左侧 4 格
    b.spawnNext();
    b.lockCurrent();
    CHECK(b.getScore() == 1);
    for (int c = 0; c < Board::WIDTH; ++c) CHECK(b.at(19, c) == 0);
    std::printf("\n");
}

int countCells(const Board& b) {
    int n = 0;
    for (int r = 0; r < Board::HEIGHT; ++r)
        for (int c = 0; c < Board::WIDTH; ++c) n += b.at(r, c) ? 1 : 0;
    return n;
}

bool rowEmpty(const Board& b, int row) {
    for (int c = 0; c < Board::WIDTH; ++c)
        if (b.at(row, c)) return false;
    return true;
}

void testClearTwoLines() {
    std::printf("== 一次消 2 行 = 3 分 ==\n");
    Board b(0, "test");
    presetExceptCol0(b, 18);   // 9 格
    presetExceptCol0(b, 19);   // 9 格
    b.setNext(std::make_unique<Block>(SHAPE_I, -1, 17, GREEN, 1));  // 竖 I 占第 0 列 16..19 行
    b.spawnNext();
    b.lockCurrent();
    CHECK(b.getScore() == 3);         // 1+2

    // 原来 18+4=22 格，消掉 2 行（20 格）后应剩 2 格：
    // 竖 I 在第 0 列 16/17 行的两格被整体下移，落到第 18/19 行第 0 列。
    CHECK(countCells(b) == 2);
    CHECK(b.at(19, 0) == 1);
    CHECK(b.at(18, 0) == 1);
    for (int r = 0; r < 18; ++r) CHECK(rowEmpty(b, r));          // 上方必须清空并已下移
    for (int r = 18; r < Board::HEIGHT; ++r)
        for (int c = 1; c < Board::WIDTH; ++c) CHECK(b.at(r, c) == 0);  // 除第 0 列外无残留
    std::printf("\n");
}

void testFourLineClear() {
    std::printf("== 一次消 4 行 = 10 分 ==\n");
    Board b(0, "test");
    for (int row = Board::HEIGHT - 4; row < Board::HEIGHT; ++row) presetExceptCol0(b, row);
    b.setNext(std::make_unique<Block>(SHAPE_I, -1, 17, GREEN, 1));  // 竖 I 一次补齐 4 行
    b.spawnNext();
    b.lockCurrent();
    CHECK(b.getScore() == 10);        // 1+2+3+4
    std::printf("\n");
}

void testGameOverByTopRow() {
    std::printf("== 结束判定：顶行被占 ==\n");
    Board b(0, "test");
    for (int c = 0; c < Board::WIDTH; ++c) b.setAt(0, c, 1);
    CHECK(b.isTopOut());
    CHECK(b.gameOver() == false);     // 还没通知过，但下一次锁定/生成会判负
    b.setNext(std::make_unique<Block>(SHAPE_I, 3, 0, GREEN, 0));
    b.spawnNext();
    CHECK(b.gameOver());              // 出生点被占直接判负（旧版会重叠几帧）
    std::printf("\n");
}

void testSpawnBlocked() {
    std::printf("== 结束判定：出生点被占 ==\n");
    Board b(0, "test");
    // O 块出生在 (4,0)-(5,1)，把出生区堵死
    b.setAt(0, 4, 1); b.setAt(0, 5, 1);
    b.setAt(1, 4, 1); b.setAt(1, 5, 1);
    b.setNext(std::make_unique<Block>(SHAPE_O, 4, 0, BLUE, 0));
    b.spawnNext();
    CHECK(b.gameOver());
    std::printf("\n");
}

void testLockWritesCells() {
    std::printf("== 锁定：Z 块 4 个格子都写进棋盘 ==\n");
    Board b(0, "test");
    b.setNext(std::make_unique<Block>(SHAPE_Z, 3, 5, LIGHTCYAN, 0));
    b.spawnNext();
    b.lockCurrent();
    // Z state0: (0,0),(1,0),(1,1),(2,1)
    CHECK(b.at(5, 3) == 1);
    CHECK(b.at(5, 4) == 1);
    CHECK(b.at(6, 4) == 1);
    CHECK(b.at(6, 5) == 1);
    std::printf("\n");
}

}  // namespace

int main() {
    testBounds();
    testRotateAtTop();
    testCollisionWithStack();
    testClearOneLine();
    testClearTwoLines();
    testFourLineClear();
    testGameOverByTopRow();
    testSpawnBlocked();
    testLockWritesCells();

    if (g_failed) {
        std::printf("\n%d 个断言失败\n", g_failed);
        return 1;
    }
    std::printf("全部通过\n");
    return 0;
}
