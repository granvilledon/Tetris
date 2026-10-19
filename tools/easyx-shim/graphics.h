// graphics.h —— EasyX 兼容层（GDI 实现）
//
// 用途：官方 EasyX 只支持 MSVC，为了让本项目能在 MinGW / CI 里编译和跑起来，
// 这里用 GDI + GDI+ 实现了游戏实际用到的那一小撮 EasyX API。
//
// 不追求完全对齐 EasyX 语义，只保证：同名的函数、同名的常量、同样的调用方式。
// 想用官方 EasyX（VS 里）时，不要把这个目录加进 include path 即可。
#pragma once

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#ifndef NOMINMAX
#define NOMINMAX
#endif

#include <windows.h>
#include <mmsystem.h>
#include <deque>
#include <string>

// EasyX 的 _T：Dev-C++/MinGW 版 EasyX 是 ANSI 版（TCHAR = char），_T 就是原样
#ifndef _T
#define _T(x) x
#endif

// BGI 调色板（EasyX 沿用的 16 色常量，COLORREF 是 BGR 排列）
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

#ifndef TRANSPARENT
#define TRANSPARENT 1
#endif
#ifndef OPAQUE
#define OPAQUE 2
#endif

// ---------------------------------------------------------------- IMAGE
// EasyX 的 IMAGE 是可拷贝的（项目里用 vector<IMAGE> 存 27 张整页图），
// 所以这里做成深拷贝：拷贝构造时复制一份 DIB，互不影响。
class IMAGE {
public:
    IMAGE() = default;
    IMAGE(const IMAGE& other);
    IMAGE& operator=(const IMAGE& other);
    ~IMAGE();

    HBITMAP bmp = nullptr;
    int width = 0;
    int height = 0;
};

// ---------------------------------------------------------------- 窗口 / 绘制
void initgraph(int width, int height, int flag = 0);
void closegraph();

void cleardevice();
void setbkcolor(COLORREF color);
void setfillcolor(COLORREF color);
void setlinecolor(COLORREF color);
void settextcolor(COLORREF color);
void setbkmode(int mode);
void settextstyle(int height, int width, const wchar_t* face);
void settextstyle(int height, int width, const char* face);

void solidrectangle(int left, int top, int right, int bottom);
void fillrectangle(int left, int top, int right, int bottom);
void rectangle(int left, int top, int right, int bottom);
void line(int x1, int y1, int x2, int y2);

void outtextxy(int x, int y, const wchar_t* text);
void outtextxy(int x, int y, const char* text);

void putimage(int x, int y, IMAGE* img);
void loadimage(IMAGE* img, const wchar_t* path, int width = 0, int height = 0);
void loadimage(IMAGE* img, const char* path, int width = 0, int height = 0);

void BeginBatchDraw();
void EndBatchDraw();
void FlushBatchDraw();

// ---------------------------------------------------------------- 输入
struct MOUSEMSG {
    UINT uMsg = 0;
    int x = 0;
    int y = 0;
};

bool MouseHit();
MOUSEMSG GetMouseMsg();

// EasyX 的 InputBox：弹一个输入框，取消时 buf 为空串
bool InputBox(char* buf, int size, const char* prompt,
              const char* title = nullptr, const char* defaultText = nullptr);
bool InputBox(wchar_t* buf, int size, const wchar_t* prompt,
              const wchar_t* title = nullptr, const wchar_t* defaultText = nullptr);
