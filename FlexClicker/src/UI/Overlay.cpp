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

        SIZE textSize;
        GetTextExtentPoint32A(hdc, text.c_str(), (int)text.length(), &textSize);

        int paddingX = ScalePixels(10, hwnd);
        int paddingY = ScalePixels(10, hwnd);

        int newWidth = textSize.cx + paddingX;
        int newHeight = textSize.cy + paddingY;

        RECT rect;
        GetWindowRect(hwnd, &rect);

        SetWindowPos(hwnd, NULL, 0, 0, newWidth, newHeight, SWP_NOMOVE | SWP_NOZORDER | SWP_NOACTIVATE | SWP_NOREDRAW);

        UpdateOverlayPosition();

        RECT rc;
        GetClientRect(hwnd, &rc);
        FillRect(hdc, &rc, (HBRUSH)GetStockObject(BLACK_BRUSH));

        SetTextColor(hdc, clickerActive ? RGB(0, 255, 0) : RGB(255, 0, 0));
        SetBkMode(hdc, TRANSPARENT);

        int textOffsetX = paddingX / 2;
        int textOffsetY = (rc.bottom - textSize.cy) / 2;

        TextOutA(hdc, textOffsetX, textOffsetY, text.c_str(), (int)text.length());   
        
        SelectObject(hdc, hOldFont);
        DeleteObject(hOverlayFont);
        EndPaint(hwnd, &ps);
    } break;
    case WM_NCHITTEST: return HTCAPTION;
    default: return DefWindowProcA(hwnd, uMsg, wParam, lParam);
    }
    return 0;
}