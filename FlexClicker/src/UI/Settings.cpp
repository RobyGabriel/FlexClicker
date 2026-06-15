#include "../Globals.h"
#include <commctrl.h>
#pragma comment(lib, "comctl32.lib")

LRESULT CALLBACK GroupBoxSubclassProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam, UINT_PTR uIdSubclass, DWORD_PTR dwRefData) {
    if (uMsg == WM_PAINT && isDarkMode) {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hwnd, &ps);

        HFONT hOldFont = (HFONT)SelectObject(hdc, hFont);

        RECT rc;
        GetClientRect(hwnd, &rc);

        FillRect(hdc, &rc, hBrushDarkBg);

        char text[128];
        GetWindowTextA(hwnd, text, sizeof(text));
        SIZE textSize = { 0 };
        GetTextExtentPoint32A(hdc, text, strlen(text), &textSize);

        HPEN hPen = CreatePen(PS_SOLID, 1, colorDarkElement);
        HPEN hOldPen = (HPEN)SelectObject(hdc, hPen);
        HBRUSH hOldBrush = (HBRUSH)SelectObject(hdc, GetStockObject(NULL_BRUSH));

        int topOffset = textSize.cy / 2;
        Rectangle(hdc, rc.left, rc.top + topOffset, rc.right, rc.bottom);

        SetTextColor(hdc, colorDarkElement);
        SetBkColor(hdc, colorDarkBg);
        SetBkMode(hdc, OPAQUE);

        TextOutA(hdc, rc.left + 10, rc.top, text, strlen(text));

        SelectObject(hdc, hOldPen);
        SelectObject(hdc, hOldBrush);
        SelectObject(hdc, hOldFont);
        DeleteObject(hPen);

        EndPaint(hwnd, &ps);
        return 0;
    }
    return DefSubclassProc(hwnd, uMsg, wParam, lParam);
}

LRESULT CALLBACK SettingsProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    if (uMsg == WM_GETDLGCODE) return DLGC_WANTALLKEYS;
    switch (uMsg) {
    case WM_CREATE: {
        isSettingsOpen = true;

        HWND g1 = CreateWindowA("BUTTON", "Input Configuration", WS_VISIBLE | WS_CHILD | BS_GROUPBOX, 5, 5, 255, 65, hwnd, NULL, NULL, NULL);
        hBtnMode = CreateWindowA("BUTTON", (currentMode == InputMode::MOUSE ? "Mode: MOUSE" : "Mode: KEYBOARD"), WS_VISIBLE | WS_CHILD, 15, 25, 235, 30, hwnd, (HMENU)15, NULL, NULL);

        HWND g2 = CreateWindowA("BUTTON", "Hotkeys", WS_VISIBLE | WS_CHILD | BS_GROUPBOX, 5, 75, 255, 85, hwnd, NULL, NULL, NULL);
        CreateWindowA("STATIC", "Toggle (ON/OFF):", WS_VISIBLE | WS_CHILD, 15, 95, 120, 20, hwnd, NULL, NULL, NULL);
        hBtnToggleSet = CreateWindowA("BUTTON", GetKeyName(toggleKey).c_str(), WS_VISIBLE | WS_CHILD, 165, 92, 80, 25, hwnd, (HMENU)10, NULL, NULL);

        hSwitchLabel = CreateWindowA("STATIC", "Switch (L/R): RCTRL+", WS_VISIBLE | WS_CHILD, 15, 123, 145, 20, hwnd, NULL, NULL, NULL);
        hBtnSwitchSet = CreateWindowA("BUTTON", GetKeyName(switchKey).c_str(), WS_VISIBLE | WS_CHILD, 165, 120, 80, 25, hwnd, (HMENU)11, NULL, NULL);

        hLabelKeySet = CreateWindowA("STATIC", "Simulated Key:", WS_VISIBLE | WS_CHILD, 15, 123, 120, 20, hwnd, NULL, NULL, NULL);
        hBtnKeySet = CreateWindowA("BUTTON", GetKeyName(selectedKey).c_str(), WS_VISIBLE | WS_CHILD, 165, 120, 80, 25, hwnd, (HMENU)14, NULL, NULL);

        HWND g3 = CreateWindowA("BUTTON", "Behavior", WS_VISIBLE | WS_CHILD | BS_GROUPBOX, 5, 165, 255, 65, hwnd, NULL, NULL, NULL);
        hCheckOverlay = CreateWindowA("BUTTON", "Show Screen Overlay", WS_VISIBLE | WS_CHILD | BS_AUTOCHECKBOX, 15, 185, 200, 20, hwnd, (HMENU)12, NULL, NULL);
        SendMessage(hCheckOverlay, BM_SETCHECK, showOverlay ? BST_CHECKED : BST_UNCHECKED, 0);

        hCheckJitter = CreateWindowA("BUTTON", "Enable Jitter (Randomness)", WS_VISIBLE | WS_CHILD | BS_AUTOCHECKBOX, 15, 205, 200, 20, hwnd, (HMENU)13, NULL, NULL);
        SendMessage(hCheckJitter, BM_SETCHECK, useJitter ? BST_CHECKED : BST_UNCHECKED, 0);

        HWND g4 = CreateWindowA("BUTTON", "Appearance", WS_VISIBLE | WS_CHILD | BS_GROUPBOX, 5, 235, 255, 85, hwnd, NULL, NULL, NULL);
        hRadioDark = CreateWindowA("BUTTON", "Dark Theme", WS_VISIBLE | WS_CHILD | BS_AUTORADIOBUTTON, 20, 257, 150, 20, hwnd, (HMENU)16, NULL, NULL);
        hRadioLight = CreateWindowA("BUTTON", "Light Theme", WS_VISIBLE | WS_CHILD | BS_AUTORADIOBUTTON, 20, 285, 150, 20, hwnd, (HMENU)17, NULL, NULL);
        SendMessage(isDarkMode ? hRadioDark : hRadioLight, BM_SETCHECK, BST_CHECKED, 0);

        SetWindowSubclass(g1, GroupBoxSubclassProc, 101, 0);
        SetWindowSubclass(g2, GroupBoxSubclassProc, 102, 0);
        SetWindowSubclass(g3, GroupBoxSubclassProc, 103, 0);
        SetWindowSubclass(g4, GroupBoxSubclassProc, 104, 0);

        bool isKeyboard = (currentMode == InputMode::KEYBOARD);
        ShowWindow(hLabelKeySet, isKeyboard ? SW_SHOW : SW_HIDE);
        ShowWindow(hBtnKeySet, isKeyboard ? SW_SHOW : SW_HIDE);
        ShowWindow(hSwitchLabel, isKeyboard ? SW_HIDE : SW_SHOW);
        ShowWindow(hBtnSwitchSet, isKeyboard ? SW_HIDE : SW_SHOW);

        ApplyTheme(hwnd);
        break;
    }
    case WM_COMMAND:
        if (LOWORD(wParam) == 10) {
            waitingForToggleKey = true;
            SetWindowTextA(hBtnToggleSet, "PRESS A KEY...");
            SetFocus(hwnd);
        }
        if (LOWORD(wParam) == 11) {
            waitingForSwitchKey = true;
            SetWindowTextA(hBtnSwitchSet, "PRESS A KEY...");
            SetFocus(hwnd);
        }
        if (LOWORD(wParam) == 12) {
            showOverlay = (SendMessage(hCheckOverlay, BM_GETCHECK, 0, 0) == BST_CHECKED);
            ShowWindow(hOverlay, showOverlay ? SW_SHOW : SW_HIDE);
        }
        if (LOWORD(wParam) == 13) {
            useJitter = (SendMessage(hCheckJitter, BM_GETCHECK, 0, 0) == BST_CHECKED);
        }
        if (LOWORD(wParam) == 14) {
            waitingForKeyboardKey = true;
            SetWindowTextA(hBtnKeySet, "PRESS A KEY...");
            SetFocus(hwnd);
        }
        if (LOWORD(wParam) == 15) {
            currentMode = (currentMode == InputMode::MOUSE) ? InputMode::KEYBOARD : InputMode::MOUSE;
            bool isKeyboard = (currentMode == InputMode::KEYBOARD);
            SetWindowTextA(hBtnMode, isKeyboard ? "Mode: KEYBOARD" : "Mode: MOUSE");
            ShowWindow(hLabelKeySet, isKeyboard ? SW_SHOW : SW_HIDE);
            ShowWindow(hBtnKeySet, isKeyboard ? SW_SHOW : SW_HIDE);
            ShowWindow(hBtnSwitchSet, isKeyboard ? SW_HIDE : SW_SHOW);
            ShowWindow(hSwitchLabel, isKeyboard ? SW_HIDE : SW_SHOW);
        }
        if (LOWORD(wParam) == 16) {
            if (SendMessage(hRadioDark, BM_GETCHECK, 0, 0) == BST_CHECKED) {
                isDarkMode = true;
                ApplyTheme(hwnd);
                ApplyTheme(hMainWnd);
            }
        }
        if (LOWORD(wParam) == 17) {
            if (SendMessage(hRadioLight, BM_GETCHECK, 0, 0) == BST_CHECKED) {
                isDarkMode = false;
                ApplyTheme(hwnd);
                ApplyTheme(hMainWnd);
            }
        }
        break;
    case WM_SETFOCUS:
        break;
    case WM_CTLCOLORSTATIC: {
        HDC hdcStatic = (HDC)wParam;
        HWND hwndStatic = (HWND)lParam;
        if (isDarkMode) {
            SetBkMode(hdcStatic, TRANSPARENT);
            char className[256];
            GetClassNameA(hwndStatic, className, 256);
            if (_stricmp(className, "Button") == 0) {
                LONG style = GetWindowLong(hwndStatic, GWL_STYLE);
                if ((style & BS_TYPEMASK) == BS_GROUPBOX) {
                    SetTextColor(hdcStatic, colorDarkElement);
                }
                else {
                    SetTextColor(hdcStatic, colorDarkText);
                }
            }
            else {
                SetTextColor(hdcStatic, colorDarkText);
            }
            SetBkColor(hdcStatic, colorDarkBg);
            return (INT_PTR)hBrushDarkBg;
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
    case WM_CTLCOLOREDIT: {
        HDC hdcEdit = (HDC)wParam;
        if (isDarkMode) {
            SetTextColor(hdcEdit, colorDarkText);
            SetBkColor(hdcEdit, colorDarkElement);
            return (INT_PTR)hBrushDarkElement;
        }
        break;
    }
    case WM_KEYDOWN:
        if (waitingForToggleKey) {
            toggleKey = (int)wParam;
            waitingForToggleKey = false;
            SetWindowTextA(hBtnToggleSet, GetKeyName(toggleKey).c_str());
        }
        else if (waitingForSwitchKey) {
            switchKey = (int)wParam;
            waitingForSwitchKey = false;
            SetWindowTextA(hBtnSwitchSet, GetKeyName(switchKey).c_str());
        }
        else if (waitingForKeyboardKey) {
            selectedKey = (int)wParam;
            waitingForKeyboardKey = false;
            SetWindowTextA(hBtnKeySet, GetKeyName(selectedKey.load()).c_str());
        }
        return 0;
    case WM_SYSKEYDOWN:
        if (waitingForToggleKey) {
            toggleKey = (int)wParam;
            waitingForToggleKey = false;
            SetWindowTextA(hBtnToggleSet, GetKeyName(toggleKey).c_str());
            GetAsyncKeyState(toggleKey.load());
        }
        else if (waitingForSwitchKey) {
            switchKey = (int)wParam;
            waitingForSwitchKey = false;
            SetWindowTextA(hBtnSwitchSet, GetKeyName(switchKey).c_str());
            GetAsyncKeyState(switchKey.load());
        }
        else if (waitingForKeyboardKey) {
            selectedKey = (int)wParam;
            waitingForKeyboardKey = false;
            SetWindowTextA(hBtnKeySet, GetKeyName(selectedKey).c_str());
        }
        break;
    case WM_DESTROY: isSettingsOpen = false; break;
    default: return DefWindowProcA(hwnd, uMsg, wParam, lParam);
    }
    return 0;
}