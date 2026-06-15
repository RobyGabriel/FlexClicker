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
    char name[64];
    UINT sc = MapVirtualKeyA(vk, MAPVK_VK_TO_VSC);
    if (GetKeyNameTextA(sc << 16, name, 64)) return std::string(name);
    return std::to_string(vk) + " key";
}
