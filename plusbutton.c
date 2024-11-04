#include <windows.h>
#include <stdio.h>

// 창 프로시저 (WindowProcedure)
LRESULT CALLBACK WindowProcedure(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam) {
    switch (message) {
    case WM_CREATE: {
        // 버튼 1 생성
        CreateWindowW(L"BUTTON", L"클릭하세요", WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,
            50, 130, 100, 30, hwnd, (HMENU)1, (HINSTANCE)GetWindowLongPtr(hwnd, GWLP_HINSTANCE), NULL); //버튼위치 수정 x,y,넓이, 높이
        //버튼 2 생성
        CreateWindowW(L"Button",L"클릭", WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,
         240, 130, 100, 30, hwnd, (HMENU)2, (HINSTANCE)GetWindowLongPtr(hwnd, GWLP_HINSTANCE), NULL);
        break;
    }
    case WM_COMMAND:
        if (LOWORD(wParam) == 1) {
            MessageBoxW(hwnd, L"버튼1이 눌렸습니다!", L"알림", MB_OK);
            }
            else if (LOWORD(wParam) == 2){
                MessageBoxW(hwnd, L"버튼2가 눌렸습니다!", L"알림", MB_OK);
            }
        if (LOWORD(wParam) == 1 || LOWORD(wParam) == 2){
            DestroyWindow(hwnd);
        }
        break;
    case WM_PAINT: {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hwnd, &ps);

        const wchar_t* text = L"한국어 확인.";
        SIZE textSize;
        GetTextExtentPoint32W(hdc, text, wcslen(text), &textSize);

        RECT clientRect;
        GetClientRect(hwnd, &clientRect);

        int x = (clientRect.right - textSize.cx) / 2;
        int y = (clientRect.bottom - textSize.cy) / 2;

        TextOutW(hdc, x, y, text, wcslen(text));
        EndPaint(hwnd, &ps);
        break;
    }
    case WM_CLOSE:
        DestroyWindow(hwnd);
        break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    case WM_TIMER:
        KillTimer(hwnd, 1);
        DestroyWindow(hwnd);
        break;
    default:
        return DefWindowProc(hwnd, message, wParam, lParam);
    }
    return 0;
}

void AnimateWindowUp(HWND hwnd, int startPosX, int startPosY, int endPosY, int speed) {
    for (int posY = startPosY; posY > endPosY; posY -= speed) {
        SetWindowPos(hwnd, NULL, startPosX, posY, 0, 0, SWP_NOZORDER | SWP_NOSIZE);
        Sleep(10);
    }
}

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nShowCmd) {
    FreeConsole();

    WNDCLASSW wc = { 0 };
    wc.lpfnWndProc = WindowProcedure;
    wc.hInstance = hInstance;
    wc.lpszClassName = L"MyWindowClass";
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);

    if (!RegisterClassW(&wc)) {
        MessageBoxW(NULL, L"Window class registration failed!", L"Error", MB_ICONEXCLAMATION | MB_OK);
        return 1;
    }

    int screenWidth = GetSystemMetrics(SM_CXSCREEN);
    int screenHeight = GetSystemMetrics(SM_CYSCREEN);
    int windowWidth = 400;
    int windowHeight = 200;
    int startPosX = screenWidth - windowWidth;
    int startPosY = screenHeight;
    int endPosY = screenHeight - windowHeight - 50;

    HWND hwnd = CreateWindowW(L"MyWindowClass", L"원하는 알림창의 제목", WS_OVERLAPPEDWINDOW,
        startPosX, startPosY, windowWidth, windowHeight, NULL, NULL, hInstance, NULL);

    if (hwnd == NULL) {
        MessageBoxW(NULL, L"Window creation failed!", L"Error", MB_ICONEXCLAMATION | MB_OK);
        return 1;
    }

    ShowWindow(hwnd, SW_SHOW);
    UpdateWindow(hwnd);

    AnimateWindowUp(hwnd, startPosX, startPosY, endPosY, 5);

    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return 0;
}
