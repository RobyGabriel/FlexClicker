#include "Globals.h"
#include <dwmapi.h>
#include <uxtheme.h>

#pragma comment(lib, "dwmapi.lib")
#pragma comment(lib, "uxtheme.lib")

#ifndef DWMWA_USE_IMMERSIVE_DARK_MODE
#define DWMWA_USE_IMMERSIVE_DARK_MODE 20
#endif

std::atomic<bool>      clickerActive(false);
std::atomic<bool>      isLeftClick(true);
std::atomic<int>       cps(10);
std::atomic<int>       toggleKey(VK_PAUSE);
std::atomic<int>       switchKey(VK_OEM_5);
std::atomic<bool>      isSettingsOpen(false);
std::atomic<bool>      showOverlay(true);
std::atomic<bool>      useJitter(false);
std::atomic<InputMode> currentMode(InputMode::MOUSE);
std::atomic<int>       selectedKey(VK_SPACE);

bool isDarkMode            = true;
bool waitingForKeyboardKey = false;
bool waitingForToggleKey   = false;
bool waitingForSwitchKey   = false;

HWND hMainWnd         = NULL;
HWND hOverlay         = NULL;
HWND hStatusLabel     = NULL;
HWND hCurrentCPSLabel = NULL;
HWND hEditCPS         = NULL;
HWND hTypeLabel       = NULL;
HWND hClickLabel      = NULL;
HWND hLabelKeySet     = NULL;
HWND hSwitchLabel     = NULL;
HWND hBtnMode         = NULL;
HWND hBtnKeySet       = NULL;
HWND hBtnToggleSet    = NULL;
HWND hBtnSwitchSet    = NULL;
HWND hCheckOverlay    = NULL;
HWND hCheckJitter     = NULL;
HWND hRadioDark       = NULL;
HWND hRadioLight      = NULL;

COLORREF colorDarkBg      = RGB(40, 40, 40);
COLORREF colorDarkElement = RGB(30, 30, 30);
COLORREF colorDarkText    = RGB(240, 240, 240);
COLORREF colorLightBg     = RGB(255, 255, 255);
COLORREF colorLightText   = RGB(0, 0, 0);

HBRUSH hBrushRed          = NULL;
HBRUSH hBrushGreen        = NULL;
HBRUSH hBrushDarkBg       = NULL;
HBRUSH hBrushDarkElement  = NULL;
HBRUSH hBrushLightBg      = NULL;
HBRUSH hBrushEditDark     = NULL;

void InitBrushes() {
    hBrushRed         = CreateSolidBrush(RGB(255, 0, 0));
    hBrushGreen       = CreateSolidBrush(RGB(0, 255, 0));
    hBrushDarkBg      = CreateSolidBrush(colorDarkBg);
    hBrushDarkElement = CreateSolidBrush(colorDarkElement);
    hBrushLightBg     = CreateSolidBrush(colorLightBg);
    hBrushEditDark    = CreateSolidBrush(RGB(45, 45, 45));
}

void CleanupBrushes() {
    DeleteObject(hBrushRed);
    DeleteObject(hBrushGreen);
    DeleteObject(hBrushDarkBg);
    DeleteObject(hBrushDarkElement);
    DeleteObject(hBrushLightBg);
    DeleteObject(hBrushEditDark);
}

void ApplyTheme(HWND hwnd) {
    if (!hwnd) return;
    
    BOOL value = isDarkMode ? TRUE : FALSE;
    DwmSetWindowAttribute(hwnd, DWMWA_USE_IMMERSIVE_DARK_MODE, &value, sizeof(value));
    
    HBRUSH hbr = isDarkMode ? hBrushDarkBg : hBrushLightBg;
    SetClassLongPtr(hwnd, GCLP_HBRBACKGROUND, (LONG_PTR)hbr);

    const wchar_t* themeName = isDarkMode ? L"DarkMode_Explorer" : NULL;
    
    EnumChildWindows(hwnd, [](HWND child, LPARAM lp) -> BOOL {
        const wchar_t* tName = (const wchar_t*)lp;
        SetWindowTheme(child, tName, NULL);
        RedrawWindow(child, NULL, NULL, RDW_INVALIDATE | RDW_UPDATENOW | RDW_ERASE);
        return TRUE;
    }, (LPARAM)themeName);

    SetWindowPos(hwnd, NULL, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_NOZORDER | SWP_FRAMECHANGED);
    RedrawWindow(hwnd, NULL, NULL, RDW_INVALIDATE | RDW_UPDATENOW | RDW_ERASE | RDW_ALLCHILDREN);
}

std::string GetKeyName(int vk) {
    if (vk >= '0' && vk <= '9') return std::string(1, (char)vk);
    if (vk >= 'A' && vk <= 'Z') return std::string(1, (char)vk);

    switch (vk) {
        case VK_LBUTTON:  return "L-Click";
        case VK_RBUTTON:  return "R-Click";
        case VK_MBUTTON:  return "M-Click";
        case VK_XBUTTON1: return "X1-Click";
        case VK_XBUTTON2: return "X2-Click";
        case VK_BACK:     return "Backspace";
        case VK_TAB:      return "Tab";
        case VK_RETURN:   return "Enter";
        case VK_SHIFT:    return "Shift";
        case VK_CONTROL:  return "Ctrl";
        case VK_MENU:     return "Alt";
        case VK_PAUSE:    return "Pause";
        case VK_CAPITAL:  return "Caps Lock";
        case VK_ESCAPE:   return "Esc";
        case VK_SPACE:    return "Space";
        case VK_PRIOR:    return "Page Up";
        case VK_NEXT:     return "Page Down";
        case VK_END:      return "End";
        case VK_HOME:     return "Home";
        case VK_LEFT:     return "Left";
        case VK_UP:       return "Up";
        case VK_RIGHT:    return "Right";
        case VK_DOWN:     return "Down";
        case VK_SNAPSHOT: return "Print Screen";
        case VK_INSERT:   return "Insert";
        case VK_DELETE:   return "Delete";
        case VK_LWIN:     return "L-Win";
        case VK_RWIN:     return "R-Win";
        case VK_NUMPAD0:  return "Num 0";
        case VK_NUMPAD1:  return "Num 1";
        case VK_NUMPAD2:  return "Num 2";
        case VK_NUMPAD3:  return "Num 3";
        case VK_NUMPAD4:  return "Num 4";
        case VK_NUMPAD5:  return "Num 5";
        case VK_NUMPAD6:  return "Num 6";
        case VK_NUMPAD7:  return "Num 7";
        case VK_NUMPAD8:  return "Num 8";
        case VK_NUMPAD9:  return "Num 9";
        case VK_MULTIPLY: return "Num *";
        case VK_ADD:      return "Num +";
        case VK_SUBTRACT: return "Num -";
        case VK_DECIMAL:  return "Num .";
        case VK_DIVIDE:   return "Num /";
        case VK_OEM_1:    return ";";
        case VK_OEM_PLUS: return "+";
        case VK_OEM_COMMA:return ",";
        case VK_OEM_MINUS:return "-";
        case VK_OEM_PERIOD:return ".";
        case VK_OEM_2:    return "/";
        case VK_OEM_3:    return "~";
        case VK_OEM_4:    return "[";
        case VK_OEM_5:    return "\\";
        case VK_OEM_6:    return "]";
        case VK_OEM_7:    return "'";
    }

    if (vk >= VK_F1 && vk <= VK_F24) return "F" + std::to_string(vk - VK_F1 + 1);

    char name[64];
    UINT sc = MapVirtualKeyA(vk, MAPVK_VK_TO_VSC);
    if (sc != 0 && GetKeyNameTextA(sc << 16, name, 64)) return std::string(name);

    return "Key " + std::to_string(vk);
}
