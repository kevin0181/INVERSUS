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
#include "Block.h"
#include "gamePlay.h"

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

    static std::vector<Block> blocks;
    static Block mainBlock(RGB(0, 0, 0), RGB(0, 0, 0), { 0,0, 50, 50 }, false);
    static CImage mainResp;
    static int c_n = 0; //response 이미지

    std::wstring mainRespW[12] = {
            L"img/gamePlay/spawn/1.png",
            L"img/gamePlay/spawn/2.png",
            L"img/gamePlay/spawn/3.png",
            L"img/gamePlay/spawn/4.png",
            L"img/gamePlay/spawn/5.png",
            L"img/gamePlay/spawn/6.png",
            L"img/gamePlay/spawn/7.png",
            L"img/gamePlay/spawn/8.png",
            L"img/gamePlay/spawn/9.png",
            L"img/gamePlay/spawn/10.png",
            L"img/gamePlay/spawn/11.png",
            L"img/gamePlay/spawn/12.png",
    };

    switch (uMsg)
    {
    case WM_CREATE:
    {   
        GetClientRect(hWnd, &rect);
        gameStateManager.setCurrentState(GameState::START);
        gameStateManager.setImage(L"img/Inversus Intro.png");
        PlayMP3(L"sound/main intro.mp3");
        mainResp.Load(mainRespW[c_n].c_str());
        break;
    }
    case WM_COMMAND:
        break;
    case WM_KEYUP:
        if (gameStateManager.getState() == GameState::GAMEPLAY) { // game start

            switch (wParam)
            {
            case 65:
                mainBlock.left = false;
                break;
            case 68:
                mainBlock.right = false;
                break;
            case 87:
                mainBlock.up = false;
                break;
            case 83:
                mainBlock.down = false;
                break;
            default:
                break;
            }
            InvalidateRect(hWnd, NULL, false);
        }

        break;
    case WM_KEYDOWN:  // 키보드 키가 눌렸을 때

        if (wParam == VK_ESCAPE) { //esc -> 설정
            if (gameStateManager.getState() == GameState::GAMEPLAY) { // game play -> setting
                if (gameUi.countDown != 1) // 게임이 시작되면 가능하게
                    break;
                KillTimer(hWnd, 1);
                setting.setting(wParam, hWnd);
                gameStateManager.setCurrentState(GameState::SETTING);
                break;
            }
            if (gameStateManager.getState() == GameState::SETTING) { // setting -> game play
                SetTimer(hWnd, 1, 1, NULL);
                gameStateManager.setImage(L"img/gamePlay/score bar.png");
                gameStateManager.setCurrentState(GameState::GAMEPLAY);
                break;
            }
            InvalidateRect(hWnd, NULL, false);
            break;
        }

        if (gameStateManager.getState() == GameState::SETTING) { //setting 상태일때 움직임
            setting.setting(wParam, hWnd);
            InvalidateRect(hWnd, NULL, false);
            break;
        }

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
            levelSetting.level_setting(wParam, hWnd, rect, mainBlock, blocks);
            gameUi.setBlackBlock(blocks, gameUi.cellSize); // 검정 블럭 설정
            blankMain(blocks, &mainBlock); // 빈 부분 만들기

            SetTimer(hWnd, 2, 100, NULL); // 죽고 난 뒤 생성 타이머

            InvalidateRect(hWnd, NULL, false);
            break;
        }

        if (gameStateManager.getState() == GameState::GAMEPLAY) { // game start
            
            switch (wParam)
            {
            case 65:
                mainBlock.left = true;
                break;
            case 68:
                mainBlock.right = true;
                break;
            case 87:
                mainBlock.up = true;
                break;
            case 83:
                mainBlock.down = true;
                break;
            default:
                break;
            }
            InvalidateRect(hWnd, NULL, false);
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
        case '1':
            mainBlock.status = false;
            SetTimer(hWnd, 2, 100, NULL); // 죽고 난 뒤 생성 타이머
            break;
        default:
            break;
        }
        InvalidateRect(hWnd, NULL, false);
        break;
    case WM_PAINT:
    {
        
        hDC = BeginPaint(hWnd, &ps);
        mDC = CreateCompatibleDC(hDC);
        hBitmap = CreateCompatibleBitmap(hDC, rect.right, rect.bottom);
        SelectObject(mDC, (HBITMAP)hBitmap);
        FillRect(mDC, &rect, (HBRUSH)GetStockObject(WHITE_BRUSH));

        gameStateManager.DrawImage(mDC, rect); // 전체배경

        if (gameStateManager.getState() == GameState::GAMEPLAY) {

            for (int x = 0; x <= gameUi.line_size; ++x) {
                MoveToEx(mDC, 0, x * gameUi.cellSize + 130, NULL);
                LineTo(mDC, rect.right, x * gameUi.cellSize + 130);
            }

            for (int y = 0; y <= gameUi.line_size; ++y) {
                MoveToEx(mDC, y * gameUi.cellSize, 130, NULL);
                LineTo(mDC, y * gameUi.cellSize, gameUi.gameBordRect.bottom);
            }

            gameUi.printBlackBlock(blocks, mDC);
            gameUi.drawGameUI(mDC, gameUi, rect);

            if (mainBlock.status) { //살아 있을 경우
                gameUi.mainAsset(mDC, rect, mainBlock);
            }
            else { //죽고 난 뒤, 리스폰
                resRet(mDC, { 0,0,50,50 }, mainResp);
            }
                
           
        }

        if (gameStateManager.getState() == GameState::SETTING) { //setting draw
            setting.draw_setting(mDC, rect);
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
        case 1: //game Play
            if (mainBlock.left) {
                OffsetRect(&mainBlock.rect, -mainBlock.speed, 0);
                OffsetRect(&mainBlock.rect, checkCrash(blocks, mainBlock), 0);
            }

            if (mainBlock.right) {
                OffsetRect(&mainBlock.rect, mainBlock.speed, 0);
                OffsetRect(&mainBlock.rect, -checkCrash(blocks, mainBlock), 0);
            }

            if (mainBlock.up) {
                OffsetRect(&mainBlock.rect, 0, -mainBlock.speed);
                OffsetRect(&mainBlock.rect, 0, checkCrash(blocks, mainBlock));
            }
            
            if (mainBlock.down) {
                OffsetRect(&mainBlock.rect, 0, mainBlock.speed);
                OffsetRect(&mainBlock.rect, 0, -checkCrash(blocks, mainBlock));
            }

            break;
        case 2:
        {
            static int count = 0;
            c_n++;
            if (c_n == 12) {
                c_n = 0;
                count++;
            }

            if (count == 3) {
                count = 0;
                KillTimer(hWnd, 2);
                break;
            }

            mainResp.Destroy();
            mainResp.Load(mainRespW[c_n].c_str());
            break;
        }
        case 10: //count down
            setCountDown(gameUi, hWnd, mainBlock);
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