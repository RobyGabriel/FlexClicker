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

        SetWindowPos(hwnd, NULL, 0, 0, ScalePixels(320, hwnd), ScalePixels(510, hwnd), SWP_NOMOVE | SWP_NOZORDER);

        HWND g1 = CreateWindowA("BUTTON", "Input Configuration", WS_VISIBLE | WS_CHILD | BS_GROUPBOX,
            ScalePixels(10, hwnd), ScalePixels(5, hwnd), ScalePixels(285, hwnd), ScalePixels(75, hwnd), hwnd, NULL, NULL, NULL);

        hBtnMode = CreateWindowA("BUTTON", (currentMode == InputMode::MOUSE ? "Mode: MOUSE" : "Mode: KEYBOARD"), WS_VISIBLE | WS_CHILD,
            ScalePixels(20, hwnd), ScalePixels(28, hwnd), ScalePixels(265, hwnd), ScalePixels(35, hwnd), hwnd, (HMENU)15, NULL, NULL);

        HWND g2 = CreateWindowA("BUTTON", "Hotkeys", WS_VISIBLE | WS_CHILD | BS_GROUPBOX,
            ScalePixels(10, hwnd), ScalePixels(90, hwnd), ScalePixels(285, hwnd), ScalePixels(115, hwnd), hwnd, NULL, NULL, NULL);

        CreateWindowA("STATIC", "Toggle (ON/OFF):", WS_VISIBLE | WS_CHILD | SS_CENTERIMAGE,
            ScalePixels(20, hwnd), ScalePixels(118, hwnd), ScalePixels(160, hwnd), ScalePixels(25, hwnd), hwnd, NULL, NULL, NULL);

        hBtnToggleSet = CreateWindowA("BUTTON", GetKeyName(toggleKey).c_str(), WS_VISIBLE | WS_CHILD,
            ScalePixels(175, hwnd), ScalePixels(115, hwnd), ScalePixels(110, hwnd), ScalePixels(30, hwnd), hwnd, (HMENU)10, NULL, NULL);

        hSwitchLabel = CreateWindowA("STATIC", "Switch (L/R): RCTRL +", WS_VISIBLE | WS_CHILD | SS_CENTERIMAGE,
            ScalePixels(20, hwnd), ScalePixels(160, hwnd), ScalePixels(170, hwnd), ScalePixels(25, hwnd), hwnd, NULL, NULL, NULL);

        hBtnSwitchSet = CreateWindowA("BUTTON", GetKeyName(switchKey).c_str(), WS_VISIBLE | WS_CHILD,
            ScalePixels(175, hwnd), ScalePixels(157, hwnd), ScalePixels(110, hwnd), ScalePixels(30, hwnd), hwnd, (HMENU)11, NULL, NULL);

        hLabelKeySet = CreateWindowA("STATIC", "Simulated Key:", WS_VISIBLE | WS_CHILD | SS_CENTERIMAGE,
            ScalePixels(20, hwnd), ScalePixels(160, hwnd), ScalePixels(160, hwnd), ScalePixels(25, hwnd), hwnd, NULL, NULL, NULL);

        hBtnKeySet = CreateWindowA("BUTTON", GetKeyName(selectedKey).c_str(), WS_VISIBLE | WS_CHILD,
            ScalePixels(175, hwnd), ScalePixels(157, hwnd), ScalePixels(110, hwnd), ScalePixels(30, hwnd), hwnd, (HMENU)14, NULL, NULL);

        HWND g3 = CreateWindowA("BUTTON", "Behavior", WS_VISIBLE | WS_CHILD | BS_GROUPBOX,
            ScalePixels(10, hwnd), ScalePixels(215, hwnd), ScalePixels(285, hwnd), ScalePixels(150, hwnd), hwnd, NULL, NULL, NULL);

        hCheckJitter = CreateWindowA("BUTTON", "Enable Jitter", WS_VISIBLE | WS_CHILD | BS_AUTOCHECKBOX | BS_VCENTER,
            ScalePixels(20, hwnd), ScalePixels(237, hwnd), ScalePixels(260, hwnd), ScalePixels(25, hwnd), hwnd, (HMENU)13, NULL, NULL);
        SendMessage(hCheckJitter, BM_SETCHECK, useJitter ? BST_CHECKED : BST_UNCHECKED, 0);

        hCheckAudio = CreateWindowA("BUTTON", "Enable Audio Cues", WS_VISIBLE | WS_CHILD | BS_AUTOCHECKBOX | BS_VCENTER,
            ScalePixels(20, hwnd), ScalePixels(267, hwnd), ScalePixels(260, hwnd), ScalePixels(25, hwnd), hwnd, (HMENU)19, NULL, NULL);
        SendMessage(hCheckAudio, BM_SETCHECK, useAudio ? BST_CHECKED : BST_UNCHECKED, 0);

        hCheckOverlay = CreateWindowA("BUTTON", "Show Screen Overlay", WS_VISIBLE | WS_CHILD | BS_AUTOCHECKBOX | BS_VCENTER,
            ScalePixels(20, hwnd), ScalePixels(297, hwnd), ScalePixels(260, hwnd), ScalePixels(25, hwnd), hwnd, (HMENU)12, NULL, NULL);
        SendMessage(hCheckOverlay, BM_SETCHECK, showOverlay ? BST_CHECKED : BST_UNCHECKED, 0);

        CreateWindowA("STATIC", "Overlay Location:", WS_VISIBLE | WS_CHILD | SS_CENTERIMAGE,
            ScalePixels(20, hwnd), ScalePixels(327, hwnd), ScalePixels(130, hwnd), ScalePixels(25, hwnd), hwnd, NULL, NULL, NULL);

        hComboPos = CreateWindowA("COMBOBOX", NULL, WS_VISIBLE | WS_CHILD | CBS_DROPDOWNLIST | WS_VSCROLL,
            ScalePixels(175, hwnd), ScalePixels(327, hwnd), ScalePixels(110, hwnd), ScalePixels(150, hwnd), hwnd, (HMENU)18, NULL, NULL);

        SendMessageA(hComboPos, CB_ADDSTRING, 0, (LPARAM)"Top Left");
        SendMessageA(hComboPos, CB_ADDSTRING, 0, (LPARAM)"Top Right");
        SendMessageA(hComboPos, CB_ADDSTRING, 0, (LPARAM)"Bottom Left");
        SendMessageA(hComboPos, CB_ADDSTRING, 0, (LPARAM)"Bottom Right");

        SendMessageA(hComboPos, CB_SETCURSEL, (WPARAM)currentOverlayPos.load(), 0);

        HWND g4 = CreateWindowA("BUTTON", "Appearance", WS_VISIBLE | WS_CHILD | BS_GROUPBOX,
            ScalePixels(10, hwnd), ScalePixels(375, hwnd), ScalePixels(285, hwnd), ScalePixels(85, hwnd), hwnd, NULL, NULL, NULL);

        hRadioDark = CreateWindowA("BUTTON", "Dark Theme", WS_VISIBLE | WS_CHILD | BS_AUTORADIOBUTTON | BS_VCENTER,
            ScalePixels(20, hwnd), ScalePixels(397, hwnd), ScalePixels(220, hwnd), ScalePixels(25, hwnd), hwnd, (HMENU)16, NULL, NULL);

        hRadioLight = CreateWindowA("BUTTON", "Light Theme", WS_VISIBLE | WS_CHILD | BS_AUTORADIOBUTTON | BS_VCENTER,
            ScalePixels(20, hwnd), ScalePixels(427, hwnd), ScalePixels(220, hwnd), ScalePixels(25, hwnd), hwnd, (HMENU)17, NULL, NULL);
        SendMessage(isDarkMode ? hRadioDark : hRadioLight, BM_SETCHECK, BST_CHECKED, 0);

        EnumChildWindows(hwnd, [](HWND hChild, LPARAM lp) -> BOOL {
            SendMessage(hChild, WM_SETFONT, (WPARAM)hFont, TRUE);
            return TRUE;
            }, 0);

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

            if (hOverlay) {
                RedrawWindow(hOverlay, NULL, NULL, RDW_INVALIDATE | RDW_UPDATENOW);
            }
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

            if (hOverlay) {
                RedrawWindow(hOverlay, NULL, NULL, RDW_INVALIDATE | RDW_UPDATENOW);
            }
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
        if (LOWORD(wParam) == 18 && HIWORD(wParam) == CBN_SELCHANGE) {
            int selectedIndex = (int)SendMessageA(hComboPos, CB_GETCURSEL, 0, 0);
            if (selectedIndex != CB_ERR) {
                currentOverlayPos = static_cast<OverlayPos>(selectedIndex);

                if (hOverlay) {
                    RedrawWindow(hOverlay, NULL, NULL, RDW_INVALIDATE | RDW_UPDATENOW);
                }
            }
        }
        if (LOWORD(wParam) == 19) {
            useAudio = (SendMessage((HWND)lParam, BM_GETCHECK, 0, 0) == BST_CHECKED);
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
            GetAsyncKeyState(toggleKey.load());
        }
        else if (waitingForSwitchKey) {
            switchKey = (int)wParam;
            waitingForSwitchKey = false;
            SetWindowTextA(hBtnSwitchSet, GetKeyName(switchKey).c_str());
            GetAsyncKeyState(toggleKey.load());
        }
        else if (waitingForKeyboardKey) {
            selectedKey = (int)wParam;
            waitingForKeyboardKey = false;
            SetWindowTextA(hBtnKeySet, GetKeyName(selectedKey.load()).c_str());

            if (hOverlay) {
                RedrawWindow(hOverlay, NULL, NULL, RDW_INVALIDATE | RDW_UPDATENOW);
            }
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

            if (hOverlay) {
                RedrawWindow(hOverlay, NULL, NULL, RDW_INVALIDATE | RDW_UPDATENOW);
            }
        }
        break;
    case WM_DESTROY: isSettingsOpen = false; break;
    default: return DefWindowProcA(hwnd, uMsg, wParam, lParam);
    }
    return 0;
}