#define WIN32_LEAN_AND_MEAN
#include <SDKDDKVer.h>
#include "resource.h"
#include "universe.h"
#include <tchar.h>

#define MAX_LOADSTRING 100

// Global variables:
HINSTANCE hInst;
WCHAR szTitle[MAX_LOADSTRING]       = _T("Across the Universe");
WCHAR szWindowClass[MAX_LOADSTRING] = _T("Universe");

HWND MainWnd;

//Submit declarations of functions included in this code module:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

Universe universe;

int APIENTRY wWinMain(
    _In_     HINSTANCE hInstance,
    _In_opt_ HINSTANCE hPrevInstance,
    _In_     LPWSTR    lpCmdLine,
    _In_     int       nCmdShow
) {
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // Global string initialize
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_ACROSSTHEUNIVERSE, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // Application initialize
    if (!InitInstance (hInstance, nCmdShow)) {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_ACROSSTHEUNIVERSE));

    MSG msg;

    SetTimer(MainWnd, 0, 1, 0);

    // Main message loop
    while (GetMessage(&msg, nullptr, 0, 0)) {
        if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg)) {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    KillTimer(MainWnd, 0);

    return (int) msg.wParam;
}

/**
 * @fn MyRegisterClass
 * @brief Register a window class
 */
ATOM MyRegisterClass(HINSTANCE hInstance) {
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_ACROSSTHEUNIVERSE));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = 0; // (HBRUSH)(COLOR_WINDOW + 1);
    wcex.lpszMenuName   = 0; // MAKEINTRESOURCEW(IDC_ACROSSTHEUNIVERSE);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

/**
 * @fn InitInstance
 * @brief Saves the instance handle and creates the main window
 * @param[in] hInstance
 * @param[in] nCmdShow
 * @note In this function, the instance handle is stored in a global variable,
 * and the main program window is created and displayed.
 */
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow) {
   hInst = hInstance; // Store the instance marker in a global variable

   MainWnd = CreateWindowEx(
       WS_EX_TOPMOST,
       szWindowClass,
       szWindowClass,
       WS_POPUP | WS_VISIBLE,
       0,
       0,
       universe.GetWidth(),
       universe.GetHeigth(),
       NULL,
       NULL,
       hInstance,
       NULL
   );

   if (!MainWnd) {
      return FALSE;
   }

   ShowWindow(MainWnd, nCmdShow);
   UpdateWindow(MainWnd);

   return TRUE;
}

/**
 * @fn WndProc
 * @brief Processes messages in the main window.
 * @param[in] hWnd
 * @param[in] message
 * @param[in] wParam
 * @param[in] lParam
 *
 * @note WM_COMMAND  - process application menu
 * @note WM_PAINT    - Drawing the main window
 * @note WM_DESTROY  - send exit message and return
 */
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
    static HDC compdc;
    static HBITMAP buffer;

    switch (message) {
        case WM_CREATE: {
            HDC hdc = GetDC(hWnd);
            compdc = CreateCompatibleDC(hdc);
            buffer = CreateCompatibleBitmap(hdc, universe.GetWidth(), universe.GetHeigth());
            SelectObject(compdc, buffer);
            ReleaseDC(hWnd, hdc);
        }
        break;
        case WM_KEYDOWN:
            if (wParam == VK_ESCAPE)  SendMessage(hWnd, WM_CLOSE, 0, 0);
            break;
        case WM_TIMER:
            InvalidateRect(hWnd, 0, FALSE);
        case WM_COMMAND: {
            int wmId = LOWORD(wParam);
            // Parse menu selection:
            switch (wmId) {
                case IDM_ABOUT:
                    DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
                    break;
                case IDM_EXIT:
                    DestroyWindow(hWnd);
                    break;
                default:
                    return DefWindowProc(hWnd, message, wParam, lParam);
            }
        }
        break;
        case WM_PAINT:
            universe.DrawingMainWindow(hWnd, compdc);
            break;
        case WM_DESTROY:
            DeleteObject(compdc);
            DeleteObject(buffer);
            PostQuitMessage(0);
            break;
        default:
            return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

// Message handler for the "About" window.
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam) {
    UNREFERENCED_PARAMETER(lParam);
    switch (message) {
    case WM_INITDIALOG:
        return (INT_PTR)TRUE;

    case WM_COMMAND:
        if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL) {
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        break;
    }
    return (INT_PTR)FALSE;
}
