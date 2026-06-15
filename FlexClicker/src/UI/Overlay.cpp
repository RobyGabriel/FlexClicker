#include "../Globals.h"

LRESULT CALLBACK OverlayProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    switch (uMsg) {
    case WM_PAINT: {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hwnd, &ps);

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
        TextOutA(hdc, 5, 5, text.c_str(), (int)text.length());
        EndPaint(hwnd, &ps);
    } break;
    case WM_NCHITTEST: return HTCAPTION;
    default: return DefWindowProcA(hwnd, uMsg, wParam, lParam);
    }
    return 0;
}