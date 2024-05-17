#include <Windows.h>
#include <tchar.h>
#include <random>
#include <vector>
#include <string>
#include <set>

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
uniform_int_distribution<int> uid_red_speed(1, 2);

LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {

    PAINTSTRUCT ps;
    HDC hDC, mDC;
    HBITMAP hBitmap;

    static RECT rect;

    static vector<Block> blocks;
    static Block mainBlock(RGB(0, 0, 0), RGB(0, 0, 0), { 0,0, 50, 50 }, false); //player 1
    static vector<Bullet> mainBullets; //player 1 bullet
    static int vk_count = 0; //press bullet count
    static bool vk_status = false;

    static int c_n = 0; //response 이미지
    static int r_n = 0;
  
    static vector<Block> redBlocks;

    switch (uMsg)
    {
    case WM_CREATE:
    {   
        GetClientRect(hWnd, &rect);
        gameStateManager.setCurrentState(GameState::START);
        gameStateManager.setImage(L"img/Inversus Intro.png");
        PlayMP3(L"sound/main intro.mp3");

        for (int i = 0; i < 6; ++i) {
            Bullet bullet;
            mainBullets.push_back(bullet);
        }

        SetTimer(hWnd, 6, 100, NULL); // press bullet timer

        break;
    }
    case WM_COMMAND:
        break;
    case WM_KEYUP:
        if (gameStateManager.getState() == GameState::GAMEPLAY) { // game start

            if (mainBlock.status) {
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
                case VK_LEFT:
                {
                    Bullet* b = nullptr;
                    if (findFalseBullet(mainBullets, b) && b != nullptr) {
                        bulletScaleDown(b, mainBlock);
                        b->left = true;
                        b->bullet_move_status = true;
                        if (vk_count >= 15 && vk_status) {
                            b->speed = b->max_speed;
                        }
                    }
                    vk_count = 0;
                    vk_status = false;
                    break;
                }
                case VK_RIGHT:
                {
                    Bullet* b = nullptr;
                    if (findFalseBullet(mainBullets, b) && b != nullptr) {
                        bulletScaleDown(b, mainBlock);
                        b->right = true;
                        b->bullet_move_status = true;
                        if (vk_count >= 15 && vk_status) {
                            b->speed = b->max_speed;
                        }
                    }
                    vk_count = 0;
                    vk_status = false;
                    break;
                }
                case VK_UP:
                {
                    Bullet* b = nullptr;
                    if (findFalseBullet(mainBullets, b) && b != nullptr) {
                        bulletScaleDown(b, mainBlock);
                        b->up = true;
                        b->bullet_move_status = true;
                        if (vk_count >= 15 && vk_status){
                            b->speed = b->max_speed;
                        }
                    }
                    vk_count = 0;
                    vk_status = false;
                    break;
                }
                case VK_DOWN:
                {
                    Bullet* b = nullptr;
                    if (findFalseBullet(mainBullets, b) && b != nullptr) {
                        bulletScaleDown(b, mainBlock);
                        b->down = true;
                        b->bullet_move_status = true;
                        if (vk_count >= 15 && vk_status) {
                            b->speed = b->max_speed;
                        }
                    }
                    vk_count = 0;
                    vk_status = false;
                    break;
                }
                default:
                    break;
                }
            }
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
                InvalidateRect(hWnd, NULL, false);
                break;
            }
            if (gameStateManager.getState() == GameState::SETTING) { // setting -> game play
                SetTimer(hWnd, 1, 1, NULL);
                gameStateManager.setImage(L"img/gamePlay/score bar.png");
                gameStateManager.setCurrentState(GameState::GAMEPLAY);
                InvalidateRect(hWnd, NULL, false);
                break;
            }
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
            //blankMain(blocks, &mainBlock, redBlocks); // 빈 부분 만들기
            redBlocks.clear();
            mainBlock.status = false;
            InvalidateRect(hWnd, NULL, false);
            break;
        }

        if (gameStateManager.getState() == GameState::GAMEPLAY) { // game start
            
            if (mainBlock.status) {
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
                case VK_LEFT:
                    vk_status = true;
                    break;
                case VK_RIGHT:
                    vk_status = true;
                    break;
                case VK_UP:
                    vk_status = true;
                    break;
                case VK_DOWN:
                    vk_status = true;
                    break;
                default:
                    break;
                }
            }
            
        }

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

            
            for (auto& redB : redBlocks) {
                if (redB.status) {
                    redB.print_red_Block(mDC, redB);
                }
                else {
                    redB.print_red_res(mDC, redB, r_n, gameUi.cellSize);
                }
            }

            if (mainBlock.status) { //살아 있을 경우
                gameUi.mainAsset(mDC, mainBlock);

                for (auto& bullet : mainBullets) { // bullet
                    bullet.print(mDC, bullet);
                }

            }
            else { //죽고 난 뒤, 리스폰
                mainBlock.print_main_res(mDC, mainBlock, c_n, gameUi.cellSize);
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
        case 1: //main block move

            if (mainBlock.status) {
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

                if (moveRedBlock(redBlocks, mainBlock, mDC)) { // redBlock이 mainBlock을 향해감 + main + red충돌체크
                    mainBlock.rect = { 0,0,50,50 };
                    OffsetRect(&mainBlock.rect, (gameUi.gameBordRect.right / 2) - 25, (gameUi.gameBordRect.bottom / 2) + 30);
                    SetTimer(hWnd, 2, 100, NULL); // 죽고 난 뒤 생성 타이머
                }

                moveChangeBackgroundBlack(redBlocks, blocks); //redBlock이 지나가는 자리는 black으로 바꿈

                for (int i = 0; i < mainBullets.size(); ++i) { // 총알 발사 등.
                    if (mainBullets[i].bullet_move_status) {
                        if (mainBullets[i].left) {
                            OffsetRect(&mainBullets[i].rect, -mainBullets[i].speed, 0);
                        }

                        if (mainBullets[i].right) {
                            OffsetRect(&mainBullets[i].rect, mainBullets[i].speed, 0);
                        }

                        if (mainBullets[i].up) {
                            OffsetRect(&mainBullets[i].rect, 0, -mainBullets[i].speed);
                        }

                        if (mainBullets[i].down) {
                            OffsetRect(&mainBullets[i].rect, 0, mainBullets[i].speed);
                        }

                        checkBulletBlock(mainBullets[i], blocks); // 총알 + 검은 블록 충돌 검사
                        
                        if (checkRedBlockBullet(mainBullets[i], redBlocks, blocks, mDC, gameUi)) { // 총알 + 빨간 블록 충돌 검사
                            //만약 충돌된 상태면 총알 지워버리기
                            mainBullets.erase(mainBullets.begin() + i);
                        }

                    }
                }

                InvalidateRect(hWnd, NULL, false);
            }
            break;
        case 2: // main block resp 타이머
        {
            static int count = 0;
            c_n++;
            if (c_n == 12) {
                c_n = 0;
                count++;
            }

            if (count == 3) {
                count = 0;
                mainBlock.status = true;
                blankMain(blocks, &mainBlock, redBlocks); // 빈 부분 만들기
                KillTimer(hWnd, 2);
                break;
            }

            InvalidateRect(hWnd, NULL, false);
            break;
        }
        case 3: //red block resp 타이머
        {
            static int r_count = 0;
            r_n++;
            if (r_n == 12) {
                r_n = 0;
                r_count++;
            }

            if (r_count == 3) {
                r_count = 0;
                for (Block& redB : redBlocks) {
                    redB.status = true;
                }
                KillTimer(hWnd, 3);
                break;
            }
            break;
        }
        case 4: //red block 생성 타이머
            if (mainBlock.status && redBlocks.size() <= 0) { // 메인 블럭이 활성화 되어 있는 상태에만 redBlock 생성
                uniform_int_distribution<int> uid_redBlock(1, gameStateManager.getLevel() * 2 + 2);
                uniform_int_distribution<int> uid_redBlockRect(1, blocks.size());

                int rand = uid_redBlock(gen); // redBlock 개수
                vector<int> rand_i; //redBlock 수
                set<int> uniqueNumbers; // 중복 방지를 위한 집합

                while (rand_i.size() < rand) {
                    int num = uid_redBlockRect(gen);

                    // 중복 여부 검사
                    if (uniqueNumbers.find(num) == uniqueNumbers.end()) {
                        uniqueNumbers.insert(num);
                        rand_i.push_back(num);
                    }
                }

                int cellSize = gameUi.cellSize;
                for (int i = 0; i < rand_i.size(); ++i) {
                    Block redB(RGB(255, 0, 0), RGB(0, 0, 0), blocks[rand_i[i]].rect, false);
                    redB.speed = uid_red_speed(gen);
                    redB.aroundRect = { redB.rect.left - cellSize, redB.rect.top - cellSize,redB.rect.right + cellSize,redB.rect.bottom + cellSize };
                    redBlocks.push_back(redB);
                }
                KillTimer(hWnd, 4);
                SetTimer(hWnd, 3, 100, NULL);
            }
            break;
        case 6: //press bullet
            if (vk_status) {
                vk_count++;
            }
            break;
        case 10: //count down
            setCountDown(gameUi, hWnd, mainBlock);
            break;
        default:
            break;
        }
        break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        break;
    }

    return DefWindowProc(hWnd, uMsg, wParam, lParam);
}