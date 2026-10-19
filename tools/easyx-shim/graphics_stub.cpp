// graphics_stub.cpp —— 单元测试用的空图形后端：
// Board 的逻辑测试只链接这个文件，不打开窗口、不依赖 GDI。
#include "graphics.h"

#include <cstring>

// ---- IMAGE（空实现）----
IMAGE::IMAGE(const IMAGE&) = default;
IMAGE& IMAGE::operator=(const IMAGE& other) {
    if (this != &other) {
        bmp = other.bmp;
        width = other.width;
        height = other.height;
    }
    return *this;
}
IMAGE::~IMAGE() = default;

// ---- 窗口 / 绘制（全部 no-op）----
void initgraph(int, int, int) {}
void closegraph() {}
void cleardevice() {}
void setbkcolor(COLORREF) {}
void setfillcolor(COLORREF) {}
void setlinecolor(COLORREF) {}
void settextcolor(COLORREF) {}
void setbkmode(int) {}
void settextstyle(int, int, const wchar_t*) {}
void settextstyle(int, int, const char*) {}
void solidrectangle(int, int, int, int) {}
void fillrectangle(int, int, int, int) {}
void rectangle(int, int, int, int) {}
void line(int, int, int, int) {}
void outtextxy(int, int, const wchar_t*) {}
void outtextxy(int, int, const char*) {}
void putimage(int, int, IMAGE*) {}
void loadimage(IMAGE*, const wchar_t*, int, int) {}
void loadimage(IMAGE*, const char*, int, int) {}
void BeginBatchDraw() {}
void EndBatchDraw() {}
void FlushBatchDraw() {}

// ---- 输入 ----
bool MouseHit() { return false; }
MOUSEMSG GetMouseMsg() { return MOUSEMSG{}; }
bool InputBox(char* buf, int size, const char*, const char*, const char*) {
    if (buf && size > 0) buf[0] = '\0';
    return false;
}
bool InputBox(wchar_t* buf, int size, const wchar_t*, const wchar_t*, const wchar_t*) {
    if (buf && size > 0) buf[0] = L'\0';
    return false;
}
