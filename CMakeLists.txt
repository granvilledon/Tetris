cmake_minimum_required(VERSION 3.30)
project(tetris)

set(CMAKE_CXX_STANDARD 17)

# 设置库的路径
link_directories("C:\\Program Files\\JetBrains\\CLion 2024.3.1.1\\bin\\mingw\\x86_64-w64-mingw32\\lib")

# 添加可执行文件
add_executable(tetris main.cpp
        test.cpp)

# 链接库
target_link_libraries(tetris winmm easyx)
