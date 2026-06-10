#include "Globals.h"
#include "Logic/Logic.h"
#include "UI/MainWindow.h"
#include "UI/Settings.h"
#include "UI/Overlay.h"
#include <thread>

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd) {
    std::thread t(LogicAutoclicker);
    t.detach();

    WNDCLASSA wc = { 0 }, wcs = { 0 }, wco = { 0 };
    wc.lpfnWndProc = WindowProc; wc.hInstance = hInstance; wc.lpszClassName = "FlexClicker";
    wc.hCursor = LoadCursor(NULL, IDC_ARROW); wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    RegisterClassA(&wc);

    wcs.lpfnWndProc = SettingsProc; wcs.hInstance = hInstance; wcs.lpszClassName = "FlexSettings";
    wcs.hCursor = LoadCursor(NULL, IDC_ARROW); wcs.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    RegisterClassA(&wcs);

    wco.lpfnWndProc = OverlayProc; wco.hInstance = hInstance; wco.lpszClassName = "FlexOverlay";
    wco.hCursor = LoadCursor(NULL, IDC_ARROW); wco.hbrBackground = CreateSolidBrush(RGB(0, 0, 0));
    RegisterClassA(&wco);

    HWND hwnd = CreateWindowExA(0, "FlexClicker", "FlexClicker Color", WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX,
        CW_USEDEFAULT, CW_USEDEFAULT, 230, 250, NULL, NULL, hInstance, NULL);

    hOverlay = CreateWindowExA(WS_EX_TOPMOST | WS_EX_LAYERED | WS_EX_TOOLWINDOW, "FlexOverlay", NULL, WS_POPUP,
        10, 10, 200, 30, NULL, NULL, hInstance, NULL);
    SetLayeredWindowAttributes(hOverlay, RGB(0, 0, 0), 200, LWA_COLORKEY | LWA_ALPHA);

    if (showOverlay) ShowWindow(hOverlay, SW_SHOW);

    if (hwnd == NULL) return 0;
    ShowWindow(hwnd, nShowCmd);

    MSG msg = { 0 };
    while (GetMessageA(&msg, NULL, 0, 0)) { TranslateMessage(&msg); DispatchMessageA(&msg); }
    return 0;
}