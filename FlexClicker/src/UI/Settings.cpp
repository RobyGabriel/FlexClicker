#include "../Globals.h"

LRESULT CALLBACK SettingsProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    if (uMsg == WM_GETDLGCODE) return DLGC_WANTALLKEYS;
    switch (uMsg) {
    case WM_CREATE:
        isSettingsOpen = true;
        CreateWindowA("STATIC", "Toggle ON/OFF:", WS_VISIBLE | WS_CHILD, 10, 13, 200, 20, hwnd, NULL, NULL, NULL);
        hBtnToggleSet = CreateWindowA("BUTTON", GetKeyName(toggleKey).c_str(), WS_VISIBLE | WS_CHILD, 150, 10, 100, 25, hwnd, (HMENU)10, NULL, NULL);
        hCheckOverlay = CreateWindowA("BUTTON", "Show Overlay", WS_VISIBLE | WS_CHILD | BS_AUTOCHECKBOX, 10, 60, 200, 20, hwnd, (HMENU)12, NULL, NULL);
        SendMessage(hCheckOverlay, BM_SETCHECK, showOverlay ? BST_CHECKED : BST_UNCHECKED, 0);
        hCheckJitter = CreateWindowA("BUTTON", "Enable Jitter (Random)", WS_VISIBLE | WS_CHILD | BS_AUTOCHECKBOX, 10, 85, 200, 20, hwnd, (HMENU)13, NULL, NULL);
        SendMessage(hCheckJitter, BM_SETCHECK, useJitter ? BST_CHECKED : BST_UNCHECKED, 0);
        hLabelKeySet = CreateWindowA("STATIC", "Key to simulate:", WS_VISIBLE | WS_CHILD, 10, 167, 200, 20, hwnd, NULL, NULL, NULL);
        hBtnKeySet = CreateWindowA("BUTTON", GetKeyName(selectedKey).c_str(), WS_VISIBLE | WS_CHILD, 150, 164, 100, 25, hwnd, (HMENU)14, NULL, NULL);
        hBtnMode = CreateWindowA("BUTTON", (currentMode == InputMode::MOUSE ? "Mode: MOUSE" : "Mode: KEYBOARD"), WS_VISIBLE | WS_CHILD, 10, 135, 240, 25, hwnd, (HMENU)15, NULL, NULL);
        hSwitchLabel = CreateWindowA("STATIC", "Switch L/R: RCTRL + ", WS_VISIBLE | WS_CHILD, 10, 167, 200, 20, hwnd, NULL, NULL, NULL);
        hBtnSwitchSet = CreateWindowA("BUTTON", GetKeyName(switchKey).c_str(), WS_VISIBLE | WS_CHILD, 150, 164, 100, 25, hwnd, (HMENU)11, NULL, NULL);
        if (currentMode == InputMode::MOUSE) {
            ShowWindow(hLabelKeySet, SW_HIDE);
            ShowWindow(hBtnKeySet, SW_HIDE);
        }
        break;
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
        break;
    case WM_SETFOCUS:
        break;
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