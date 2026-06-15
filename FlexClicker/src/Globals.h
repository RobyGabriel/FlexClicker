#pragma once
#include <windows.h>
#include <atomic>
#include <string>

extern std::atomic<bool> clickerActive;
extern std::atomic<bool> isLeftClick;
extern std::atomic<int> cps;
extern std::atomic<int> toggleKey;
extern std::atomic<int> switchKey;
extern std::atomic<bool> isSettingsOpen;
extern std::atomic<bool> showOverlay;
extern std::atomic<bool> useJitter;
enum class InputMode { MOUSE, KEYBOARD };
extern std::atomic<InputMode> currentMode;
extern std::atomic<int> selectedKey;
extern HWND hLabelKeySet, hOverlay, hBtnToggleSet, hBtnSwitchSet, hMainWnd;
extern HWND hCheckOverlay, hCheckJitter, hEditCPS, hStatusLabel, hCurrentCPSLabel, hTypeLabel, hBtnKeySet, hBtnMode;
extern HBRUSH hBrushRed, hBrushGreen;
extern bool waitingForToggleKey;
extern bool waitingForSwitchKey;
extern bool waitingForKeyboardKey;
extern HWND hClickLabel, hSwitchLabel;
extern bool isDarkMode;
extern HBRUSH hBrushDarkBg;
extern HBRUSH hBrushDarkElement;
extern HBRUSH hBrushLightBg;
extern HBRUSH hBrushEditDark;
extern COLORREF colorDarkBg;
extern COLORREF colorDarkElement;
extern COLORREF colorDarkText;
extern COLORREF colorLightBg;
extern COLORREF colorLightText;
extern HWND hRadioDark, hRadioLight;
extern HFONT hFont;

void InitBrushes();
void CleanupBrushes();
void ApplyTheme(HWND hwnd);
std::string GetKeyName(int vk);