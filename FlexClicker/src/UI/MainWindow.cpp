#include "../Globals.h"
#include <Commctrl.h>
#include <string>

void UpdateCPSFromEdit() {
    char buffer[10];
    GetWindowTextA(hEditCPS, buffer, sizeof(buffer));
    try {
        int newCPS = std::stoi(buffer);
        if (newCPS > 0 && newCPS <= 999) {
            cps = newCPS;
            if (hOverlay) {
                RedrawWindow(hOverlay, NULL, NULL, RDW_INVALIDATE | RDW_UPDATENOW);
            }
        }
    }
    catch (...) {}
}

LRESULT CALLBACK EditSubclassProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam, UINT_PTR uIdSubclass, DWORD_PTR dwRefData) {
    if (uMsg == WM_CHAR && wParam == VK_RETURN) {
        UpdateCPSFromEdit();
        SetFocus(GetParent(hwnd));
        return 0;
    }
    return DefSubclassProc(hwnd, uMsg, wParam, lParam);
}

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    switch (uMsg) {
    case WM_CREATE:
        hMainWnd = hwnd;

        SetWindowPos(hwnd, NULL, 0, 0, ScalePixels(290, hwnd), ScalePixels(240, hwnd), SWP_NOMOVE | SWP_NOZORDER);

        CreateWindowA("STATIC", "Set target CPS:", WS_VISIBLE | WS_CHILD | SS_CENTERIMAGE,
            ScalePixels(20, hwnd), ScalePixels(10, hwnd), ScalePixels(120, hwnd), ScalePixels(25, hwnd), hwnd, NULL, NULL, NULL);

        hEditCPS = CreateWindowA("EDIT", "10", WS_VISIBLE | WS_CHILD | WS_BORDER | ES_NUMBER | ES_CENTER | WS_TABSTOP,
            ScalePixels(150, hwnd), ScalePixels(10, hwnd), ScalePixels(110, hwnd), ScalePixels(25, hwnd), hwnd, (HMENU)101, NULL, NULL);

        SetWindowSubclass(hEditCPS, EditSubclassProc, 0, 0);

        CreateWindowA("STATIC", "Current CPS:", WS_VISIBLE | WS_CHILD | SS_CENTERIMAGE,
            ScalePixels(20, hwnd), ScalePixels(45, hwnd), ScalePixels(120, hwnd), ScalePixels(25, hwnd), hwnd, NULL, NULL, NULL);

        hCurrentCPSLabel = CreateWindowA("STATIC", "10", WS_VISIBLE | WS_CHILD | SS_CENTER | SS_CENTERIMAGE,
            ScalePixels(150, hwnd), ScalePixels(45, hwnd), ScalePixels(110, hwnd), ScalePixels(25, hwnd), hwnd, NULL, NULL, NULL);

        hClickLabel = CreateWindowA("STATIC", "Sent Command:", WS_VISIBLE | WS_CHILD | SS_CENTERIMAGE,
            ScalePixels(20, hwnd), ScalePixels(80, hwnd), ScalePixels(120, hwnd), ScalePixels(25, hwnd), hwnd, NULL, NULL, NULL);

        hTypeLabel = CreateWindowA("STATIC", "LMB", WS_VISIBLE | WS_CHILD | SS_CENTER | SS_CENTERIMAGE,
            ScalePixels(150, hwnd), ScalePixels(80, hwnd), ScalePixels(110, hwnd), ScalePixels(25, hwnd), hwnd, NULL, NULL, NULL);

        CreateWindowA("STATIC", "Status:", WS_VISIBLE | WS_CHILD | SS_CENTERIMAGE,
            ScalePixels(20, hwnd), ScalePixels(115, hwnd), ScalePixels(120, hwnd), ScalePixels(25, hwnd), hwnd, NULL, NULL, NULL);

        hStatusLabel = CreateWindowA("STATIC", "STOPPED", WS_VISIBLE | WS_CHILD | SS_CENTER | SS_CENTERIMAGE,
            ScalePixels(150, hwnd), ScalePixels(115, hwnd), ScalePixels(110, hwnd), ScalePixels(25, hwnd), hwnd, NULL, NULL, NULL);

        CreateWindowA("BUTTON", "SETTINGS", WS_VISIBLE | WS_CHILD | WS_TABSTOP,
            ScalePixels(20, hwnd), ScalePixels(155, hwnd), ScalePixels(240, hwnd), ScalePixels(32, hwnd), hwnd, (HMENU)2, NULL, NULL);

        EnumChildWindows(hwnd, [](HWND hChild, LPARAM lp) -> BOOL {
            SendMessage(hChild, WM_SETFONT, (WPARAM)hFont, TRUE);
            return TRUE;
            }, 0);

        SetTimer(hwnd, 1, 100, NULL);
        ApplyTheme(hwnd);
        break;

    case WM_TIMER:
        if (wParam == 1) {
            SetWindowTextA(hStatusLabel, clickerActive ? "RUNNING" : "STOPPED");
            SetWindowTextA(hCurrentCPSLabel, std::to_string(cps.load()).c_str());
            bool isKeyboard = (currentMode.load() == InputMode::KEYBOARD);

            if (isKeyboard) {
                SetWindowTextA(hTypeLabel, (GetKeyName(selectedKey.load())).c_str());
            }
            else {
                SetWindowTextA(hTypeLabel, isLeftClick ? "LMB" : "RMB");
            }

            InvalidateRect(hStatusLabel, NULL, TRUE);
            if (showOverlay && hOverlay) {
                RedrawWindow(hOverlay, NULL, NULL, RDW_INVALIDATE | RDW_UPDATENOW);
            }
        }
        break;

    case WM_CTLCOLORSTATIC: {
        HDC hdcStatic = (HDC)wParam;
        HWND hwndStatic = (HWND)lParam;
        if (hwndStatic == hStatusLabel) {
            if (clickerActive) {
                SetBkColor(hdcStatic, RGB(0, 255, 0));
                return (INT_PTR)hBrushGreen;
            }
            else {
                SetBkColor(hdcStatic, RGB(255, 0, 0));
                SetTextColor(hdcStatic, RGB(255, 255, 255));
                return (INT_PTR)hBrushRed;
            }
        }
        if (isDarkMode) {
            SetBkMode(hdcStatic, TRANSPARENT);
            SetTextColor(hdcStatic, colorDarkText);
            SetBkColor(hdcStatic, colorDarkBg);
            return (INT_PTR)hBrushDarkBg;
        }
        break;
    }

    case WM_CTLCOLOREDIT: {
        HDC hdcEdit = (HDC)wParam;
        if (isDarkMode) {
            SetTextColor(hdcEdit, colorDarkText);
            SetBkColor(hdcEdit, colorDarkElement);
            return (INT_PTR)hBrushDarkElement;
        }
        break;
    }

    case WM_CTLCOLORDLG: {
        if (isDarkMode) {
            return (INT_PTR)hBrushDarkBg;
        }
        break;
    }

    case WM_CTLCOLORBTN: {
        if (isDarkMode) {
            return (INT_PTR)hBrushDarkElement;
        }
        break;
    }

    case WM_COMMAND:
        if (LOWORD(wParam) == 2) {
            if (!isSettingsOpen) {
                HWND hSet = CreateWindowExA(0, "FlexSettings", "Settings", WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU,
                    500, 200, ScalePixels(300, hwnd), ScalePixels(430, hwnd), hwnd, NULL, GetModuleHandle(NULL), NULL);
                ShowWindow(hSet, SW_SHOW);
                SetFocus(hSet);
            }
        }
        break;

    case WM_DESTROY:
        KillTimer(hwnd, 1);
        PostQuitMessage(0);
        return 0;
    }
    return DefWindowProcA(hwnd, uMsg, wParam, lParam);
}