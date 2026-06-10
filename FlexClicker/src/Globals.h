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
extern HWND hLabelKeySet, hOverlay, hBtnToggleSet, hBtnSwitchSet;
extern HWND hCheckOverlay, hCheckJitter, hEditCPS, hStatusLabel, hCurrentCPSLabel, hTypeLabel, hBtnKeySet, hBtnMode;
extern HBRUSH hBrushRed, hBrushGreen;
extern bool waitingForToggleKey;
extern bool waitingForSwitchKey;
extern bool waitingForKeyboardKey;
extern HWND hClickLabel, hSwitchLabel;

std::string GetKeyName(int vk);