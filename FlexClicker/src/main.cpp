#include "Globals.h"
#include "Logic/Logic.h"
#include "UI/MainWindow.h"
#include "UI/Settings.h"
#include "UI/Overlay.h"
#include <thread>
#include "../resource.h"

#pragma comment(linker,"\"/manifestdependency:type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")

int mainWidth = ScalePixels(260, NULL);
int mainHeight = ScalePixels(320, NULL);

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd) {
    SetProcessDPIAware();
    InitBrushes();
    std::thread t(LogicAutoclicker);
    t.detach();

    WNDCLASSEXA wc = { 0 }, wcs = { 0 };
    WNDCLASSA wco = { 0 };

    wc.cbSize = sizeof(WNDCLASSEXA);
    wc.style = CS_HREDRAW | CS_VREDRAW;
    wc.lpfnWndProc = WindowProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = "FlexClicker";
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    wc.hbrBackground = hBrushDarkBg;
    wc.hIcon = LoadIconA(hInstance, MAKEINTRESOURCEA(IDI_ICON1));
    wc.hIconSm = LoadIconA(hInstance, MAKEINTRESOURCEA(IDI_ICON1));
    RegisterClassExA(&wc);

    wcs.cbSize = sizeof(WNDCLASSEXA);
    wcs.style = CS_HREDRAW | CS_VREDRAW;
    wcs.lpfnWndProc = SettingsProc;
    wcs.hInstance = hInstance;
    wcs.lpszClassName = "FlexSettings";
    wcs.hCursor = LoadCursor(NULL, IDC_ARROW);
    wcs.hbrBackground = hBrushDarkBg;
    wcs.hIcon = LoadIconA(hInstance, MAKEINTRESOURCEA(IDI_ICON1));
    wcs.hIconSm = LoadIconA(hInstance, MAKEINTRESOURCEA(IDI_ICON1));
    RegisterClassExA(&wcs);

    wco.lpfnWndProc = OverlayProc; wco.hInstance = hInstance; wco.lpszClassName = "FlexOverlay";
    wco.hCursor = LoadCursor(NULL, IDC_ARROW); wco.hbrBackground = CreateSolidBrush(RGB(0, 0, 0));
    RegisterClassA(&wco);

    HWND hwnd = CreateWindowExA(0, "FlexClicker", "FlexClicker",
        WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX,
        CW_USEDEFAULT, CW_USEDEFAULT, mainWidth, mainHeight, NULL, NULL, hInstance, NULL);

    hOverlay = CreateWindowExA(WS_EX_TOPMOST | WS_EX_LAYERED | WS_EX_TOOLWINDOW, "FlexOverlay", NULL, WS_POPUP, 10, 10, 450, 30, NULL, NULL, hInstance, NULL);
    SetLayeredWindowAttributes(hOverlay, 0, 220, LWA_ALPHA);

    if (showOverlay) ShowWindow(hOverlay, SW_SHOW);

    if (hwnd == NULL) return 0;
    ShowWindow(hwnd, nShowCmd);

    MSG msg = { 0 };
    while (GetMessageA(&msg, NULL, 0, 0)) { TranslateMessage(&msg); DispatchMessageA(&msg); }
    CleanupBrushes();
    return 0;
}