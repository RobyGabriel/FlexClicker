#include "Logic.h"
#include "../Globals.h"
#include <thread>
#include <chrono>
#include <random>

#pragma comment(lib, "winmm.lib")

void LogicAutoclicker() {
    timeBeginPeriod(1);
    std::default_random_engine generator;
    bool waitingForToggleKey = false;
    bool waitingForSwitchKey = false;

    while (true) {
        if (isSettingsOpen.load()) { clickerActive = false; Sleep(10); continue; }
        if (GetAsyncKeyState(toggleKey.load()) & 1) { clickerActive = !clickerActive; Beep(clickerActive ? 300 : 200, 50); }
        if ((GetAsyncKeyState(VK_RCONTROL) < 0) && (GetAsyncKeyState(switchKey.load()) & 1)) { isLeftClick = !isLeftClick; MessageBeep(MB_ICONINFORMATION); }

        if (clickerActive) {
            double baseDelay = 1000.0 / cps.load();
            double delayFinal = baseDelay;
            if (useJitter.load()) { std::uniform_real_distribution<double> dist(-baseDelay * 0.2, baseDelay * 0.2); delayFinal += dist(generator); }
            auto t1 = std::chrono::high_resolution_clock::now();
            if (currentMode == InputMode::MOUSE) {
                INPUT input = { 0 }; input.type = INPUT_MOUSE;
                input.mi.dwFlags = isLeftClick ? MOUSEEVENTF_LEFTDOWN : MOUSEEVENTF_RIGHTDOWN; SendInput(1, &input, sizeof(INPUT));
                input.mi.dwFlags = isLeftClick ? MOUSEEVENTF_LEFTUP : MOUSEEVENTF_RIGHTUP; SendInput(1, &input, sizeof(INPUT));
            }
            else {
                INPUT input = { 0 };
                input.type = INPUT_KEYBOARD;

                input.ki.wVk = (WORD)selectedKey.load();

                SendInput(1, &input, sizeof(INPUT));

                input.ki.dwFlags = KEYEVENTF_KEYUP;
                SendInput(1, &input, sizeof(INPUT));
            }
            while (true) {
                auto t2 = std::chrono::high_resolution_clock::now();
                if (std::chrono::duration<double, std::milli>(t2 - t1).count() >= delayFinal) break;
                Sleep(1);
            }
        }
        else Sleep(10);
    }
    timeEndPeriod(1);
}