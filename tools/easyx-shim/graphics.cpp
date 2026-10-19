// graphics.cpp —— EasyX 兼容层实现（GDI + GDI+）
#include "graphics.h"

#include <windowsx.h>
#include <objidl.h>
#include <gdiplus.h>
#include <cstring>
#include <vector>

#pragma comment(lib, "gdiplus.lib")
#pragma comment(lib, "gdi32.lib")
#pragma comment(lib, "user32.lib")

namespace {

HWND  g_hwnd = nullptr;
HDC   g_memDC = nullptr;      // 后台缓冲：所有绘制先进这里
HBITMAP g_memBmp = nullptr;
int   g_w = 0, g_h = 0;

COLORREF g_bkColor = RGB(255, 255, 255);
COLORREF g_fillColor = RGB(255, 255, 255);
COLORREF g_lineColor = RGB(255, 255, 255);
COLORREF g_textColor = RGB(0, 0, 0);
int      g_bkMode = OPAQUE;
HFONT    g_font = nullptr;

std::deque<MOUSEMSG> g_mouseQueue;

ULONG_PTR g_gdiplusToken = 0;
bool      g_gdiplusReady = false;

void ensureGdiplus() {
    if (g_gdiplusReady) return;
    Gdiplus::GdiplusStartupInput input;
    Gdiplus::GdiplusStartup(&g_gdiplusToken, &input, nullptr);
    g_gdiplusReady = true;
}

// 处理积压的窗口消息。游戏主循环不跑消息泵，只能在这里顺带抽干队列，
// 否则窗口超过 5 秒不响应，Windows 会把标题栏涂成"未响应"。
void pumpMessages() {
    MSG msg;
    while (PeekMessageW(&msg, nullptr, 0, 0, PM_REMOVE)) {
        TranslateMessage(&msg);
        DispatchMessageW(&msg);
    }
}

void present() {
    if (!g_hwnd || !g_memDC) return;
    HDC dc = GetDC(g_hwnd);
    BitBlt(dc, 0, 0, g_w, g_h, g_memDC, 0, 0, SRCCOPY);
    ReleaseDC(g_hwnd, dc);
}

std::wstring widen(const char* s) {
    if (!s) return L"";
    int need = MultiByteToWideChar(CP_UTF8, 0, s, -1, nullptr, 0);   // 源码是 UTF-8
    if (need <= 0) return L"";
    std::wstring out(need - 1, L'\0');
    MultiByteToWideChar(CP_UTF8, 0, s, -1, &out[0], need);
    return out;
}

std::string narrow(const wchar_t* s) {
    if (!s) return "";
    int need = WideCharToMultiByte(CP_UTF8, 0, s, -1, nullptr, 0, nullptr, nullptr);
    if (need <= 0) return "";
    std::string out(need - 1, '\0');
    WideCharToMultiByte(CP_UTF8, 0, s, -1, &out[0], need, nullptr, nullptr);
    return out;
}

// 独立的输入框窗口：一个静态文本 + 一个编辑框，回车确认、ESC 取消
struct InputBoxState {
    std::wstring prompt;
    std::wstring text;
    bool done = false;
    bool ok = false;
    HWND edit = nullptr;
};

LRESULT CALLBACK inputBoxProc(HWND hwnd, UINT msg, WPARAM wp, LPARAM lp) {
    auto* st = reinterpret_cast<InputBoxState*>(GetWindowLongPtrW(hwnd, GWLP_USERDATA));
    switch (msg) {
        case WM_COMMAND:
            if (LOWORD(wp) == IDOK && st) {
                int n = GetWindowTextLengthW(st->edit);
                std::wstring buf(n + 1, L'\0');
                GetWindowTextW(st->edit, &buf[0], n + 1);
                buf.resize(n);
                st->text = buf;
                st->ok = true;
                st->done = true;
                DestroyWindow(hwnd);
            } else if (LOWORD(wp) == IDCANCEL && st) {
                st->done = true;
                DestroyWindow(hwnd);
            }
            return 0;
        case WM_CLOSE:
            if (st) st->done = true;
            DestroyWindow(hwnd);
            return 0;
        default:
            return DefWindowProcW(hwnd, msg, wp, lp);
    }
}

std::wstring runInputBox(const std::wstring& prompt, const std::wstring& title,
                         const std::wstring& defaultText) {
    static bool classReady = false;
    const wchar_t* kClass = L"EasyXShimInputBox";
    HINSTANCE inst = GetModuleHandleW(nullptr);
    if (!classReady) {
        WNDCLASSW wc{};
        wc.lpfnWndProc = inputBoxProc;
        wc.hInstance = inst;
        wc.hCursor = LoadCursorW(nullptr, (LPCWSTR)IDC_ARROW);
        wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
        wc.lpszClassName = kClass;
        RegisterClassW(&wc);
        classReady = true;
    }

    InputBoxState st;
    st.prompt = prompt;
    int w = 460, h = 170;
    int sx = (GetSystemMetrics(SM_CXSCREEN) - w) / 2;
    int sy = (GetSystemMetrics(SM_CYSCREEN) - h) / 2;
    HWND dlg = CreateWindowExW(WS_EX_DLGMODALFRAME, kClass, title.c_str(),
                               WS_POPUP | WS_CAPTION | WS_SYSMENU,
                               sx, sy, w, h, nullptr, nullptr, inst, nullptr);
    if (!dlg) return L"";
    SetWindowLongPtrW(dlg, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(&st));

    CreateWindowExW(0, L"STATIC", prompt.c_str(), WS_CHILD | WS_VISIBLE,
                    16, 16, w - 40, 24, dlg, nullptr, inst, nullptr);
    st.edit = CreateWindowExW(WS_EX_CLIENTEDGE, L"EDIT", defaultText.c_str(),
                              WS_CHILD | WS_VISIBLE | WS_TABSTOP | ES_AUTOHSCROLL,
                              16, 48, w - 48, 26, dlg, (HMENU)1001, inst, nullptr);
    CreateWindowExW(0, L"BUTTON", L"OK", WS_CHILD | WS_VISIBLE | BS_DEFPUSHBUTTON,
                    w - 200, 92, 80, 30, dlg, (HMENU)IDOK, inst, nullptr);
    CreateWindowExW(0, L"BUTTON", L"Cancel", WS_CHILD | WS_VISIBLE,
                    w - 110, 92, 80, 30, dlg, (HMENU)IDCANCEL, inst, nullptr);

    ShowWindow(dlg, SW_SHOW);
    SetForegroundWindow(dlg);
    SetFocus(st.edit);

    while (!st.done) {
        MSG msg;
        if (GetMessageW(&msg, nullptr, 0, 0) <= 0) break;
        // 编辑框里按回车 = 点 OK（Edit 默认只发 WM_CHAR，不触发 IDOK）
        if (msg.message == WM_KEYDOWN && msg.wParam == VK_RETURN &&
            GetFocus() == st.edit) {
            SendMessageW(dlg, WM_COMMAND, MAKEWPARAM(IDOK, BN_CLICKED), 0);
            continue;
        }
        if (msg.message == WM_KEYDOWN && msg.wParam == VK_ESCAPE) {
            SendMessageW(dlg, WM_COMMAND, MAKEWPARAM(IDCANCEL, BN_CLICKED), 0);
            continue;
        }
        TranslateMessage(&msg);
        DispatchMessageW(&msg);
    }
    return st.ok ? st.text : L"";
}

}  // namespace

// ---------------------------------------------------------------- IMAGE
IMAGE::IMAGE(const IMAGE& other) {
    width = other.width;
    height = other.height;
    if (other.bmp && width > 0 && height > 0) {
        HDC screen = GetDC(nullptr);
        HDC src = CreateCompatibleDC(screen);
        HDC dst = CreateCompatibleDC(screen);
        HBITMAP oldSrc = (HBITMAP)SelectObject(src, other.bmp);
        bmp = CreateCompatibleBitmap(screen, width, height);
        HBITMAP oldDst = (HBITMAP)SelectObject(dst, bmp);
        BitBlt(dst, 0, 0, width, height, src, 0, 0, SRCCOPY);
        SelectObject(src, oldSrc);
        SelectObject(dst, oldDst);
        DeleteDC(src);
        DeleteDC(dst);
        ReleaseDC(nullptr, screen);
    }
}

IMAGE& IMAGE::operator=(const IMAGE& other) {
    if (this == &other) return *this;
    IMAGE tmp(other);
    std::swap(bmp, tmp.bmp);
    std::swap(width, tmp.width);
    std::swap(height, tmp.height);
    return *this;
}

IMAGE::~IMAGE() {
    if (bmp) DeleteObject(bmp);
    bmp = nullptr;
}

// ---------------------------------------------------------------- 窗口
namespace {

LRESULT CALLBACK wndProc(HWND hwnd, UINT msg, WPARAM wp, LPARAM lp) {
    switch (msg) {
        case WM_PAINT: {
            PAINTSTRUCT ps;
            HDC dc = BeginPaint(hwnd, &ps);
            if (g_memDC) BitBlt(dc, 0, 0, g_w, g_h, g_memDC, 0, 0, SRCCOPY);
            EndPaint(hwnd, &ps);
            return 0;
        }
        case WM_LBUTTONDOWN:
        case WM_LBUTTONUP:
        case WM_RBUTTONDOWN:
        case WM_RBUTTONUP:
        case WM_MOUSEMOVE: {
            MOUSEMSG m;
            m.uMsg = msg;
            m.x = GET_X_LPARAM(lp);
            m.y = GET_Y_LPARAM(lp);
            if (msg != WM_MOUSEMOVE || g_mouseQueue.empty() ||
                g_mouseQueue.back().uMsg != WM_MOUSEMOVE) {
                g_mouseQueue.push_back(m);
            } else {
                g_mouseQueue.back() = m;
            }
            return 0;
        }
        case WM_CLOSE:
            DestroyWindow(hwnd);
            return 0;
        case WM_DESTROY:
            PostQuitMessage(0);
            return 0;
        default:
            return DefWindowProcW(hwnd, msg, wp, lp);
    }
}

}  // namespace

void initgraph(int width, int height, int) {
    SetProcessDPIAware();          // 关掉缩放，窗口像素 = 绘图像素

    HINSTANCE inst = GetModuleHandleW(nullptr);
    const wchar_t* kClass = L"EasyXShimWnd";
    WNDCLASSW wc{};
    wc.lpfnWndProc = wndProc;
    wc.hInstance = inst;
    wc.hCursor = LoadCursorW(nullptr, (LPCWSTR)IDC_ARROW);
    wc.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
    wc.lpszClassName = kClass;
    RegisterClassW(&wc);

    wchar_t exePath[MAX_PATH]{};
    GetModuleFileNameW(nullptr, exePath, MAX_PATH);
    std::wstring title = exePath;
    size_t slash = title.find_last_of(L"\\/");
    if (slash != std::wstring::npos) title = title.substr(slash + 1);
    size_t dot = title.find_last_of(L'.');
    if (dot != std::wstring::npos) title = title.substr(0, dot);

    RECT rc{0, 0, width, height};
    AdjustWindowRect(&rc, WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX, FALSE);
    g_hwnd = CreateWindowExW(0, kClass, title.c_str(),
                             WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX,
                             CW_USEDEFAULT, CW_USEDEFAULT,
                             rc.right - rc.left, rc.bottom - rc.top,
                             nullptr, nullptr, inst, nullptr);
    g_w = width;
    g_h = height;

    HDC screen = GetDC(nullptr);
    g_memDC = CreateCompatibleDC(screen);
    g_memBmp = CreateCompatibleBitmap(screen, width, height);
    SelectObject(g_memDC, g_memBmp);
    ReleaseDC(nullptr, screen);

    ShowWindow(g_hwnd, SW_SHOW);
    UpdateWindow(g_hwnd);
    pumpMessages();
}

void closegraph() {
    if (g_memDC) { DeleteDC(g_memDC); g_memDC = nullptr; }
    if (g_memBmp) { DeleteObject(g_memBmp); g_memBmp = nullptr; }
    if (g_hwnd) { DestroyWindow(g_hwnd); g_hwnd = nullptr; }
    if (g_font) { DeleteObject(g_font); g_font = nullptr; }
}

// ---------------------------------------------------------------- 绘制
void setbkcolor(COLORREF color) { g_bkColor = color; }
void setfillcolor(COLORREF color) { g_fillColor = color; }
void setlinecolor(COLORREF color) { g_lineColor = color; }
void settextcolor(COLORREF color) { g_textColor = color; }
void setbkmode(int mode) { g_bkMode = mode; }

void cleardevice() {
    if (!g_memDC) return;
    RECT rc{0, 0, g_w, g_h};
    HBRUSH brush = CreateSolidBrush(g_bkColor);
    FillRect(g_memDC, &rc, brush);
    DeleteObject(brush);
}

namespace {
void drawRect(int l, int t, int r, int b, bool fill, bool stroke) {
    if (!g_memDC) return;
    if (fill) {
        HBRUSH brush = CreateSolidBrush(g_fillColor);
        RECT rc{l, t, r, b};
        FillRect(g_memDC, &rc, brush);
        DeleteObject(brush);
    }
    if (stroke) {
        HPEN pen = CreatePen(PS_SOLID, 1, g_lineColor);
        HPEN old = (HPEN)SelectObject(g_memDC, pen);
        HBRUSH oldBrush = (HBRUSH)SelectObject(g_memDC, GetStockObject(NULL_BRUSH));
        Rectangle(g_memDC, l, t, r, b);
        SelectObject(g_memDC, old);
        SelectObject(g_memDC, oldBrush);
        DeleteObject(pen);
    }
}
}  // namespace

void solidrectangle(int l, int t, int r, int b) { drawRect(l, t, r, b, true, false); }
void fillrectangle(int l, int t, int r, int b) { drawRect(l, t, r, b, true, true); }
void rectangle(int l, int t, int r, int b) { drawRect(l, t, r, b, false, true); }

void line(int x1, int y1, int x2, int y2) {
    if (!g_memDC) return;
    HPEN pen = CreatePen(PS_SOLID, 1, g_lineColor);
    HPEN old = (HPEN)SelectObject(g_memDC, pen);
    MoveToEx(g_memDC, x1, y1, nullptr);
    LineTo(g_memDC, x2, y2);
    SelectObject(g_memDC, old);
    DeleteObject(pen);
}

void settextstyle(int height, int width, const wchar_t* face) {
    if (g_font) DeleteObject(g_font);
    g_font = CreateFontW(-height, width, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE,
                         DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
                         ANTIALIASED_QUALITY, DEFAULT_PITCH | FF_DONTCARE,
                         face ? face : L"Consolas");
}

void settextstyle(int height, int width, const char* face) {
    std::wstring w = widen(face);
    settextstyle(height, width, w.c_str());
}

namespace {
void drawText(int x, int y, const std::wstring& text) {
    if (!g_memDC) return;
    HFONT oldFont = nullptr;
    if (g_font) oldFont = (HFONT)SelectObject(g_memDC, g_font);
    SetBkMode(g_memDC, g_bkMode == TRANSPARENT ? TRANSPARENT : OPAQUE);
    SetBkColor(g_memDC, g_bkColor);
    SetTextColor(g_memDC, g_textColor);
    TextOutW(g_memDC, x, y, text.c_str(), (int)text.size());
    if (oldFont) SelectObject(g_memDC, oldFont);
}
}  // namespace

void outtextxy(int x, int y, const wchar_t* text) { drawText(x, y, text ? text : L""); }
void outtextxy(int x, int y, const char* text) { drawText(x, y, widen(text)); }

void putimage(int x, int y, IMAGE* img) {
    if (!g_memDC || !img || !img->bmp) return;
    HDC src = CreateCompatibleDC(g_memDC);
    HBITMAP old = (HBITMAP)SelectObject(src, img->bmp);
    BitBlt(g_memDC, x, y, img->width, img->height, src, 0, 0, SRCCOPY);
    SelectObject(src, old);
    DeleteDC(src);
}

void loadimage(IMAGE* img, const wchar_t* path, int width, int height) {
    if (!img) return;
    ensureGdiplus();
    Gdiplus::Bitmap src(path);
    if (src.GetLastStatus() != Gdiplus::Ok) {
        OutputDebugStringA("loadimage 失败");
        return;
    }
    int w = width  > 0 ? width  : (int)src.GetWidth();
    int h = height > 0 ? height : (int)src.GetHeight();

    HDC screen = GetDC(nullptr);
    HDC dc = CreateCompatibleDC(screen);
    HBITMAP bmp = CreateCompatibleBitmap(screen, w, h);
    HBITMAP old = (HBITMAP)SelectObject(dc, bmp);
    {
        Gdiplus::Graphics g(dc);
        g.SetInterpolationMode(Gdiplus::InterpolationModeHighQualityBicubic);
        g.SetPixelOffsetMode(Gdiplus::PixelOffsetModeHighQuality);
        g.DrawImage(&src, 0, 0, w, h);
    }
    SelectObject(dc, old);
    DeleteDC(dc);
    ReleaseDC(nullptr, screen);

    img->bmp = bmp;
    img->width = w;
    img->height = h;
}

void loadimage(IMAGE* img, const char* path, int width, int height) {
    std::wstring w = widen(path);
    loadimage(img, w.c_str(), width, height);
}

void BeginBatchDraw() {}          // 一直在后台 DC 上画，不需要额外动作

void EndBatchDraw() {
    present();
    pumpMessages();
}

void FlushBatchDraw() { present(); }

// ---------------------------------------------------------------- 输入
bool MouseHit() {
    pumpMessages();
    return !g_mouseQueue.empty();
}

MOUSEMSG GetMouseMsg() {
    if (g_mouseQueue.empty()) return MOUSEMSG{};
    MOUSEMSG m = g_mouseQueue.front();
    g_mouseQueue.pop_front();
    return m;
}

bool InputBox(char* buf, int size, const char* prompt,
              const char* title, const char* defaultText) {
    if (!buf || size <= 0) return false;
    std::wstring w = runInputBox(widen(prompt), widen(title ? title : "输入"),
                                 widen(defaultText));
    std::string s = narrow(w.c_str());
    if ((int)s.size() >= size) s.resize(size - 1);
    std::strcpy(buf, s.c_str());
    return !s.empty();
}

bool InputBox(wchar_t* buf, int size, const wchar_t* prompt,
              const wchar_t* title, const wchar_t* defaultText) {
    if (!buf || size <= 0) return false;
    std::wstring w = runInputBox(prompt ? prompt : L"", title ? title : L"输入",
                                 defaultText ? defaultText : L"");
    wcsncpy(buf, w.c_str(), size - 1);
    buf[size - 1] = L'\0';
    return !w.empty();
}
