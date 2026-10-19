// common.h —— 全局常量与小工具
#pragma once

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#include <windows.h>

// BGI 调色板（EasyX 沿用的 16 色常量）
#ifndef TETRIS_BGI_COLORS
#define TETRIS_BGI_COLORS
#define BLACK        RGB(0, 0, 0)
#define BLUE         RGB(0, 0, 170)
#define GREEN        RGB(0, 170, 0)
#define CYAN         RGB(0, 170, 170)
#define RED          RGB(170, 0, 0)
#define MAGENTA      RGB(170, 0, 170)
#define BROWN        RGB(170, 85, 0)
#define LIGHTGRAY    RGB(170, 170, 170)
#define DARKGRAY     RGB(85, 85, 85)
#define LIGHTBLUE    RGB(85, 85, 255)
#define LIGHTGREEN   RGB(85, 255, 85)
#define LIGHTCYAN    RGB(85, 255, 255)
#define LIGHTRED     RGB(255, 85, 85)
#define LIGHTMAGENTA RGB(255, 85, 255)
#define YELLOW       RGB(255, 255, 85)
#define WHITE        RGB(255, 255, 255)
#endif

// 方块边长（可在设置里切换小/中/大界面）
const int SMALL_SIZE = 20;
const int MEDIUM_SIZE = 30;
const int LARGE_SIZE = 40;
inline int gridSize = MEDIUM_SIZE;
inline int halfgridSize = gridSize / 2;

// 方块形状编号
const int SHAPE_O = 0;
const int SHAPE_I = 1;
const int SHAPE_L = 2;
const int SHAPE_J = 3;
const int SHAPE_S = 4;
const int SHAPE_Z = 5;
const int SHAPE_T = 6;
const int SHAPE_COUNT = 7;

// 自定义配色
const int COLOR_BOARD = RGB(209, 150, 34);   // 已固定的格子
const int COLOR_BLOCK = RGB(255, 219, 149);  // 空格子

// 键盘按键（_getch 的第二段返回值）
const int KEY_LEFT = 75;
const int KEY_RIGHT = 77;
const int KEY_UP = 72;
const int KEY_DOWN = 80;
const int KEY_W = 119;
const int KEY_S = 115;
const int KEY_A = 97;
const int KEY_D = 100;
const int KEY_ESC = 27;

// 页面编号
const int PAGE_EXIT = 0;
const int PAGE_MAIN_MENU = 1;
const int PAGE_CREATGAME_MENU = 2;
const int PAGE_MODE_MENU = 3;
const int PAGE_GAME = 4;
const int PAGE_PAUSE_MENU = 5;
const int PAGE_GAMEOVER = 6;
const int PAGE_SAVE_MENU = 7;
const int PAGE_LEADERBOARD = 8;
const int PAGE_SETTINGS_MENU = 9;
const int PAGE_RULES = 10;
const int PAGE_MUSIC_MENU = 11;
const int PAGE_SIZE_MENU = 12;
const int PAGE_KEY_MENU = 13;

// 游戏难度（数值 = 自动下落间隔毫秒数）
const int MODE_EASY = 500;
const int MODE_NORMAL = 300;
const int MODE_HARD = 150;

// 吉祥物朝向
const int MASCOT_RIGHT = 1;
const int MASCOT_LEFT = -1;
