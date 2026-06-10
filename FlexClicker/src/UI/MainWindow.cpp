#include "../Globals.h"

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    switch (uMsg) {
    case WM_CREATE:
        hBrushRed = CreateSolidBrush(RGB(255, 0, 0));
        hBrushGreen = CreateSolidBrush(RGB(0, 255, 0));
        CreateWindowA("STATIC", "Set new CPS:", WS_VISIBLE | WS_CHILD, 20, 10, 120, 20, hwnd, NULL, NULL, NULL);
        hEditCPS = CreateWindowA("EDIT", "10", WS_VISIBLE | WS_CHILD | WS_BORDER | ES_NUMBER, 140, 10, 50, 20, hwnd, NULL, NULL, NULL);
        CreateWindowA("BUTTON", "APPLY", WS_VISIBLE | WS_CHILD, 20, 35, 170, 30, hwnd, (HMENU)1, NULL, NULL);
        CreateWindowA("BUTTON", "SETTINGS", WS_VISIBLE | WS_CHILD, 20, 70, 170, 30, hwnd, (HMENU)2, NULL, NULL);
        CreateWindowA("STATIC", "CPS:", WS_VISIBLE | WS_CHILD, 20, 110, 80, 20, hwnd, NULL, NULL, NULL);
        hCurrentCPSLabel = CreateWindowA("STATIC", "10", WS_VISIBLE | WS_CHILD, 110, 110, 50, 20, hwnd, NULL, NULL, NULL);
        CreateWindowA("STATIC", "STATUS:", WS_VISIBLE | WS_CHILD, 20, 140, 80, 20, hwnd, NULL, NULL, NULL);
        hStatusLabel = CreateWindowA("STATIC", "STOPPED", WS_VISIBLE | WS_CHILD, 110, 140, 80, 20, hwnd, NULL, NULL, NULL);
        hClickLabel = CreateWindowA("STATIC", "CLICK:", WS_VISIBLE | WS_CHILD, 20, 170, 80, 20, hwnd, NULL, NULL, NULL);
        hTypeLabel = CreateWindowA("STATIC", "LEFT", WS_VISIBLE | WS_CHILD, 110, 170, 80, 20, hwnd, NULL, NULL, NULL);
        SetTimer(hwnd, 1, 100, NULL);
        break;
    case WM_TIMER:
        if (wParam == 1) {
            SetWindowTextA(hStatusLabel, clickerActive ? "RUNNING" : "STOPPED");
            SetWindowTextA(hCurrentCPSLabel, std::to_string(cps.load()).c_str());
            bool isKeyboard = (currentMode.load() == InputMode::KEYBOARD);
            SetWindowTextA(hClickLabel, isKeyboard ? "KEY:" : "CLICK:");
            SetWindowTextA(hTypeLabel, isKeyboard ? GetKeyName(selectedKey.load()).c_str() : (isLeftClick ? "LEFT" : "RIGHT"));

            if (currentMode.load() == InputMode::KEYBOARD) {
                SetWindowTextA(hTypeLabel, (GetKeyName(selectedKey.load())).c_str());
            }
            else {
                SetWindowTextA(hTypeLabel, isLeftClick ? "LEFT" : "RIGHT");
            }

            InvalidateRect(hStatusLabel, NULL, TRUE);
            if (showOverlay) InvalidateRect(hOverlay, NULL, TRUE);
        }
        break;
    case WM_CTLCOLORSTATIC: {
        HDC hdcStatic = (HDC)wParam;
        HWND hwndStatic = (HWND)lParam;
        if (hwndStatic == hStatusLabel) {
            if (clickerActive) { SetBkColor(hdcStatic, RGB(0, 255, 0)); return (INT_PTR)hBrushGreen; }
            else { SetBkColor(hdcStatic, RGB(255, 0, 0)); SetTextColor(hdcStatic, RGB(255, 255, 255)); return (INT_PTR)hBrushRed; }
        }
    } break;
    case WM_COMMAND:
        if (LOWORD(wParam) == 1) {
            char buffer[10];
            GetWindowTextA(hEditCPS, buffer, 10);
            try { int newCPS = std::stoi(buffer); if (newCPS > 0) cps = newCPS; }
            catch (...) {}
        }
        if (LOWORD(wParam) == 2) {
            if (!isSettingsOpen) {
                HWND hSet = CreateWindowExA(0, "FlexSettings", "Settings", WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU,
                    500, 200, 280, 280, hwnd, NULL, GetModuleHandle(NULL), NULL);
                ShowWindow(hSet, SW_SHOW);
                SetFocus(hSet);
            }
        }
        break;
    case WM_DESTROY:
        DeleteObject(hBrushRed); DeleteObject(hBrushGreen);
        KillTimer(hwnd, 1); PostQuitMessage(0);
        return 0;
    }
    return DefWindowProcA(hwnd, uMsg, wParam, lParam);
}