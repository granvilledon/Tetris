cmake_minimum_required(VERSION 3.15)
project(tetris CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 游戏本体：src + EasyX 兼容层（MSVC + 官方 EasyX 时可把 shim 换成官方库）
add_executable(tetris
    src/main.cpp
    src/Block.cpp
    src/Board.cpp
    src/Leaderboard.cpp
    src/Game.cpp
    tools/easyx-shim/graphics.cpp
)
target_include_directories(tetris PRIVATE src tools/easyx-shim)
target_link_libraries(tetris PRIVATE gdi32 user32 gdiplus winmm)

# 棋盘逻辑单元测试：不依赖图形库（链接空实现）
enable_testing()
add_executable(board_tests
    tests/test_board.cpp
    src/Block.cpp
    src/Board.cpp
    tools/easyx-shim/graphics_stub.cpp
)
target_include_directories(board_tests PRIVATE src tools/easyx-shim)
add_test(NAME board_logic COMMAND board_tests)
