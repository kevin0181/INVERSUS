#include <Windows.h>
#include <tchar.h>
#include <random>
#include <vector>
#include <string>

#include "sound.h"
#include "PlayerSetting.h"
#include "GameState.h"
#include "GameStateManager.h"
#include "LevelSetting.h"
#include "GameUI.h"
#include "Global.h"
#include "CountDown.h"

using namespace std;

ULONG_PTR gdiplusToken;

HINSTANCE g_hInst;
LPCTSTR lpszClass = L"INVERSUS";
LPCTSTR lpszWindowName = L"INVERSUS";

#define WIDTH 1200
#define HEIGHT 800

LRESULT CALLBACK WndProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam);

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpszCmdParam, int nCmdShow) {

    HWND hWnd;
    MSG Message;
    WNDCLASSEX WndClass;
    g_hInst = hInstance;

    WndClass.cbSize = sizeof(WndClass);
    WndClass.style = CS_HREDRAW | CS_VREDRAW;
    WndClass.lpfnWndProc = (WNDPROC)WndProc;
    WndClass.cbClsExtra = 0;
    WndClass.cbWndExtra = 0;
    WndClass.hInstance = hInstance;
    WndClass.hIcon = LoadIcon(NULL, IDI_QUESTION);
    WndClass.hCursor = LoadCursor(NULL, IDC_HAND);
    WndClass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
    WndClass.lpszMenuName = NULL;
    WndClass.lpszClassName = lpszClass;
    WndClass.hIconSm = LoadIcon(NULL, IDI_QUESTION);
    RegisterClassEx(&WndClass);

    // 전체 화면 크기를 가져옵니다.
    int screenWidth = GetSystemMetrics(SM_CXSCREEN);
    int screenHeight = GetSystemMetrics(SM_CYSCREEN);

    // 전체화면 스타일로 창을 생성합니다.
    hWnd = CreateWindow(lpszClass, lpszWindowName, WS_POPUP | WS_VISIBLE,
        0, 0, screenWidth, screenHeight, NULL, (HMENU)NULL, hInstance, NULL);

    ShowWindow(hWnd, nCmdShow);
    UpdateWindow(hWnd);

    while (GetMessage(&Message, 0, 0, 0)) {
        TranslateMessage(&Message);
        DispatchMessage(&Message);
    }

    return Message.wParam;

}

#define WIDTH_LINE 40
#define HEGHIT_LINE 40

random_device rd;
mt19937 gen(rd());
uniform_int_distribution<int> uid_RGB(0, 255);
LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {

    PAINTSTRUCT ps;
    HDC hDC, mDC;
    HBITMAP hBitmap;

    static RECT rect;
    
    switch (uMsg)
    {
    case WM_CREATE:
    {
        gameStateManager.setCurrentState(GameState::START);
        gameStateManager.setImage(L"img/Inversus Intro.png");
        PlayMP3(L"sound/main intro.mp3");
        break;
    }
    case WM_COMMAND:
        break;
    case WM_KEYUP:
        break;
    case WM_KEYDOWN:  // 키보드 키가 눌렸을 때

        if (gameStateManager.getState() == GameState::START && wParam == VK_RETURN) { // 시작화면 -> player Select 화면
            PlaySecondMP3(L"sound/button sound.MP3"); // 버튼 사운드
            gameStateManager.setImage(L"img/player/player_0_v2.png");
            gameStateManager.setCurrentState(GameState::PLAYER);
            InvalidateRect(hWnd, NULL, false);
            break;
        }

        if (gameStateManager.getState() == GameState::PLAYER) { // 게임 시작 전 setting
            playerSetting.game_setting(wParam);
            InvalidateRect(hWnd, NULL, false);
            break;
        }

        if (gameStateManager.getState() == GameState::LEVEL) { // game level 선택
            levelSetting.level_setting(wParam, hWnd);
            InvalidateRect(hWnd, NULL, false);
            break;
        }

        if (gameStateManager.getState() == GameState::GAMEPLAY) { // game start

        }

        break;
    case WM_LBUTTONDOWN:
        break;
    case WM_LBUTTONUP:
        break;
    case WM_RBUTTONDOWN:
        break;
    case WM_RBUTTONUP:
        break;
    case WM_MOUSEMOVE:
        break;
    case WM_CHAR:
        switch (wParam)
        {
        default:
            break;
        }
        InvalidateRect(hWnd, NULL, false);
        break;
    case WM_PAINT:
    {
        GetClientRect(hWnd, &rect);
        hDC = BeginPaint(hWnd, &ps);
        mDC = CreateCompatibleDC(hDC);
        hBitmap = CreateCompatibleBitmap(hDC, rect.right, rect.bottom);
        SelectObject(mDC, (HBITMAP)hBitmap);
        FillRect(mDC, &rect, (HBRUSH)GetStockObject(WHITE_BRUSH));

        RECT gameBord = rect;

        gameStateManager.DrawImage(mDC, rect); // 전체배경

        if (gameStateManager.getState() == GameState::GAMEPLAY) {

            int line_size = (gameStateManager.getLevel() * 10);
            int cellSize = (rect.right) / line_size;

            for (int x = 0; x <= line_size; ++x) {
                if (x * cellSize + 130 > rect.bottom) {
                    gameBord.bottom = (x - 1) * cellSize + 130;
                    break;
                }
                MoveToEx(mDC, 0, x * cellSize + 130, NULL);
                LineTo(mDC, rect.right, x * cellSize + 130);
            }
            for (int y = 0; y <= line_size; ++y) {
                MoveToEx(mDC, y * cellSize, 130, NULL);
                LineTo(mDC, y * cellSize, gameBord.bottom);
            }

            gameUi.setGameBord(gameBord); //게임 보드 사이즈 설정
            gameUi.drawGameUI(mDC, gameUi, rect);
        }

        BitBlt(hDC, 0, 0, rect.right, rect.bottom, mDC, 0, 0, SRCCOPY);

        DeleteObject(hBitmap); // 생성한 비트맵 삭제
        DeleteDC(mDC);
        EndPaint(hWnd, &ps);
        break;
    }
    case WM_TIMER:
        switch (wParam)
        {
        case 10: //count down
            setCountDown(gameUi, hWnd);
            break;
        default:
            break;
        }
        InvalidateRect(hWnd, NULL, false);
        break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        break;
    }

    return DefWindowProc(hWnd, uMsg, wParam, lParam);
}