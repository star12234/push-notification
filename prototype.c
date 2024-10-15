#include <windows.h>
#include <stdio.h>
 
// 창 프로시저 (WindowProcedure)
LRESULT CALLBACK WindowProcedure(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam) {
    switch (message) {
    case WM_PAINT: {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hwnd, &ps);

        // 화면에 출력할 텍스트 (유니코드 문자열)
        const wchar_t* text = L"한국어 확인.";

        // 텍스트 크기 측정 (유니코드 버전 함수 사용)
        SIZE textSize;
        GetTextExtentPoint32W(hdc, text, wcslen(text), &textSize);

        // 창 크기 가져오기
        RECT clientRect;
        GetClientRect(hwnd, &clientRect);

        // 텍스트를 중앙에 맞추기 위한 좌표 계산
        int x = (clientRect.right - textSize.cx) / 2;  // 가로 중앙
        int y = (clientRect.bottom - textSize.cy) / 2; // 세로 중앙

        // 텍스트 출력 (유니코드 버전 함수 사용)
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
    // 창을 startPosY에서 endPosY까지 천천히 이동시키는 함수
    for (int posY = startPosY; posY > endPosY; posY -= speed) {
        SetWindowPos(hwnd, NULL, startPosX, posY, 0, 0, SWP_NOZORDER | SWP_NOSIZE);
        Sleep(10);  // 창이 움직이는 속도 조절
    }
}
 // 유니코드를 사용하기 위해 wWinmain
int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nShowCmd){
    // 콘솔 창 해제
    FreeConsole();
    // 윈도우 클래스 설정 (유니코드 문자열 사용)
    WNDCLASSW wc = { 0 };
    wc.lpfnWndProc = WindowProcedure; // 창 프로시저 연결
    wc.hInstance = hInstance;
    wc.lpszClassName = L"MyWindowClass";  // 유니코드 문자열 사용
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);  // 기본 화살표 커서 사용
    wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1); // 창 배경 설정

    // 윈도우 클래스 등록 (유니코드 버전 사용)
    if (!RegisterClassW(&wc)) {  // 유니코드 버전 사용
        MessageBoxW(NULL, L"Window class registration failed!", L"Error", MB_ICONEXCLAMATION | MB_OK);
        return 1;
    }

    // 화면 해상도 얻기
    int screenWidth = GetSystemMetrics(SM_CXSCREEN); // 화면의 가로 길이
    int screenHeight = GetSystemMetrics(SM_CYSCREEN); // 화면의 세로 길이

    // 창 크기 설정
    int windowWidth = 400;
    int windowHeight = 200;

    // 창의 시작 위치 (오른쪽 아래 화면 바깥)
    int startPosX = screenWidth - windowWidth;  // 오른쪽 모서리
    int startPosY = screenHeight;  // 화면 바깥 아래쪽
    int endPosY = screenHeight - windowHeight - 50;  // 화면 아래에서 창의 높이만큼 위

    // 윈도우 창 생성 (유니코드 버전 사용)
    HWND hwnd = CreateWindowW(L"MyWindowClass", L"원하는 알림창의 제목", WS_OVERLAPPEDWINDOW,
        startPosX, startPosY, windowWidth, windowHeight, NULL, NULL, hInstance, NULL);

    if (hwnd == NULL) {
        MessageBoxW(NULL, L"Window creation failed!", L"Error", MB_ICONEXCLAMATION | MB_OK);
        return 1;
    }
    
    // x초 이후에 종료 (5초)
    SetTimer(hwnd, 1, 5000, NULL);
    
    // 창 표시 및 업데이트
    ShowWindow(hwnd, SW_SHOW);
    UpdateWindow(hwnd);

    // 창을 위로 올리는 애니메이션
    AnimateWindowUp(hwnd, startPosX, startPosY, endPosY, 5);

    // 메시지 루프x
    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return 0;
}
