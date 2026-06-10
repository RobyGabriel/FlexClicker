#include "Globals.h"

std::atomic<bool> clickerActive(false);
std::atomic<bool> isLeftClick(true);
std::atomic<int> cps(10);
std::atomic<int> toggleKey(VK_PAUSE);
std::atomic<int> switchKey(VK_OEM_5);
std::atomic<bool> isSettingsOpen(false);
std::atomic<bool> showOverlay(true);
std::atomic<bool> useJitter(false);
std::atomic<InputMode> currentMode(InputMode::MOUSE);
std::atomic<int> selectedKey(VK_SPACE);
HWND hLabelKeySet;
HWND hEditCPS, hStatusLabel, hCurrentCPSLabel, hTypeLabel;
HWND hBtnToggleSet, hBtnSwitchSet, hCheckOverlay, hCheckJitter;
HWND hOverlay, hSwitchLabel;
HWND hBtnKeySet = NULL;
HWND hBtnMode = NULL;
HWND hClickLabel = NULL;
HBRUSH hBrushRed, hBrushGreen;

bool waitingForKeyboardKey = false;
bool waitingForToggleKey = false;
bool waitingForSwitchKey = false;

std::string GetKeyName(int vk) {
    char name[64];
    UINT sc = MapVirtualKeyA(vk, MAPVK_VK_TO_VSC);
    if (GetKeyNameTextA(sc << 16, name, 64)) return std::string(name);
    return std::to_string(vk) + " key";
}