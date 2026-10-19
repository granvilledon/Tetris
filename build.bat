@echo off
rem 一键构建 + 跑测试（需要 g++ 在 PATH，比如 msys2/mingw64）
rem 想用 CMake 也行：cmake -B build && cmake --build build

setlocal
set OUT=build

if not exist %OUT% mkdir %OUT

echo [1/3] 编译游戏本体...
g++ -std=c++17 -O2 -static -static-libgcc -static-libstdc++ ^
    src\main.cpp src\Block.cpp src\Board.cpp src\Leaderboard.cpp src\Game.cpp ^
    tools\easyx-shim\graphics.cpp ^
    -Isrc -Itools\easyx-shim ^
    -o %OUT%\tetris.exe -lgdi32 -luser32 -lgdiplus -lwinmm
if errorlevel 1 (echo 构建失败 & exit /b 1)

echo [2/3] 编译单元测试...
g++ -std=c++17 -O2 -static ^
    tests\test_board.cpp src\Block.cpp src\Board.cpp ^
    tools\easyx-shim\graphics_stub.cpp ^
    -Isrc -Itools\easyx-shim ^
    -o %OUT%\board_tests.exe
if errorlevel 1 (echo 测试构建失败 & exit /b 1)

echo [3/3] 运行测试...
%OUT%\board_tests.exe
if errorlevel 1 exit /b 1

echo.
echo 构建完成：%OUT%\tetris.exe（需要 ./picture 资源目录）
