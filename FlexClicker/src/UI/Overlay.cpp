#include "../Globals.h"

LRESULT CALLBACK OverlayProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    switch (uMsg) {
    case WM_PAINT: {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hwnd, &ps);
        int scaledFontHeight = ScalePixels(20, hwnd);

        HFONT hOverlayFont = CreateFontA(
            scaledFontHeight, 0, 0, 0,
            FW_BOLD,
            FALSE, FALSE, FALSE,
            DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
            CLEARTYPE_QUALITY, DEFAULT_PITCH | FF_DONTCARE, "Consolas"
        );

        HFONT hOldFont = (HFONT)SelectObject(hdc, hOverlayFont);

        std::string text = clickerActive ? "ON | " : "OFF | ";

        if (currentMode.load() == InputMode::KEYBOARD) {
            text += GetKeyName(selectedKey.load());
        }
        else {
            text += isLeftClick ? "LMB" : "RMB";
        }

        text += " | " + std::to_string(cps.load()) + " CPS";
        if (useJitter) text += " | Jitter";

        SetTextColor(hdc, clickerActive ? RGB(0, 255, 0) : RGB(255, 0, 0));
        SetBkMode(hdc, TRANSPARENT);
        int textOffset = ScalePixels(5, hwnd);
        TextOutA(hdc, 5, 5, text.c_str(), (int)text.length());
        SelectObject(hdc, hOldFont);
        DeleteObject(hOverlayFont);
        EndPaint(hwnd, &ps);
    } break;
    case WM_NCHITTEST: return HTCAPTION;
    default: return DefWindowProcA(hwnd, uMsg, wParam, lParam);
    }
    return 0;
}