#include "Globals.h"
#include "Logic/Logic.h"
#include "UI/MainWindow.h"
#include "UI/Settings.h"
#include "UI/Overlay.h"
#include <thread>

#pragma comment(linker,"\"/manifestdependency:type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd) {
    SetProcessDPIAware();
    InitBrushes();
    std::thread t(LogicAutoclicker);
    t.detach();

    WNDCLASSA wc = { 0 }, wcs = { 0 }, wco = { 0 };
    wc.lpfnWndProc = WindowProc; wc.hInstance = hInstance; wc.lpszClassName = "FlexClicker";
    wc.hCursor = LoadCursor(NULL, IDC_ARROW); wc.hbrBackground = hBrushDarkBg;
    RegisterClassA(&wc);

    wcs.lpfnWndProc = SettingsProc; wcs.hInstance = hInstance; wcs.lpszClassName = "FlexSettings";
    wcs.hCursor = LoadCursor(NULL, IDC_ARROW); wcs.hbrBackground = hBrushDarkBg;
    RegisterClassA(&wcs);

    wco.lpfnWndProc = OverlayProc; wco.hInstance = hInstance; wco.lpszClassName = "FlexOverlay";
    wco.hCursor = LoadCursor(NULL, IDC_ARROW); wco.hbrBackground = CreateSolidBrush(RGB(0, 0, 0));
    RegisterClassA(&wco);

    HWND hwnd = CreateWindowExA(0, "FlexClicker", "FlexClicker", WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX,
        CW_USEDEFAULT, CW_USEDEFAULT, 230, 250, NULL, NULL, hInstance, NULL);

    hOverlay = CreateWindowExA(WS_EX_TOPMOST | WS_EX_LAYERED | WS_EX_TOOLWINDOW, "FlexOverlay", NULL, WS_POPUP, 10, 10, 280, 30, NULL, NULL, hInstance, NULL);
    SetLayeredWindowAttributes(hOverlay, RGB(0, 0, 0), 200, LWA_COLORKEY | LWA_ALPHA);

    if (showOverlay) ShowWindow(hOverlay, SW_SHOW);

    if (hwnd == NULL) return 0;
    ShowWindow(hwnd, nShowCmd);

    MSG msg = { 0 };
    while (GetMessageA(&msg, NULL, 0, 0)) { TranslateMessage(&msg); DispatchMessageA(&msg); }
    CleanupBrushes();
    return 0;
}