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
#include "Explodes.h"

using namespace std;

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
uniform_int_distribution<int> uid_special_block(1, 7);
uniform_int_distribution<int> uid_red_rand(0, 1);

static double rotatingBulletAngle = 0.0;
static const double ROTATING_BULLET_RADIUS = 100.0;

void UpdateRotatingBulletPosition(Block& mainBlock, Bullet& rotatingBullet, double angle) {
    POINT center = { (mainBlock.rect.left + mainBlock.rect.right) / 2, (mainBlock.rect.top + mainBlock.rect.bottom) / 2 };
    rotatingBullet.rect.left = static_cast<LONG>(center.x + ROTATING_BULLET_RADIUS * cos(angle) - 10);
    rotatingBullet.rect.top = static_cast<LONG>(center.y + ROTATING_BULLET_RADIUS * sin(angle) - 10);
    rotatingBullet.rect.right = rotatingBullet.rect.left + 20;
    rotatingBullet.rect.bottom = rotatingBullet.rect.top + 20;
}

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
    static int reloadCnt = 0; // 재장전 쿨타임 1발씩.

    static int c_n = 0; //response 이미지
    static int r_n = 0;
  
    static int combo = 0;
    static RECT comboRect;

    static vector<Block> redBlocks;
    static vector<Block> specialBlocks;

    static Bullet rotatingBullet;
    static bool rotatingBulletStatus = false;

    static vector<Explosion> explodes;

    static int move_cnt = 0;
    static bool boardLineStatus = false;

    static vector<Bullet> dropBullet;

    switch (uMsg)
    {
    case WM_CREATE:
    {   
        GetClientRect(hWnd, &rect);
        gameStateManager.setCurrentState(GameState::START);
        gameStateManager.setImage(L"img/Inversus Intro.png");
        PlayMP3(L"sound/main intro.mp3");
        rotatingBullet.color = RGB(0, 255, 0);
        SetTimer(hWnd, 6, 100, NULL); // press bullet timer

        break;
    }
    case WM_CHAR:
        switch (wParam)
        {
        case '[':
            backgroundS -= 100;
            if (backgroundS < -10000) backgroundS = -10000; // 최소 볼륨 제한
            SetVolume(backgroundS);
            break;
        case ']':
            backgroundS += 100;
            if (backgroundS > 0) backgroundS = 0; // 최대 볼륨 제한
            SetVolume(backgroundS);
            break;
        case 'l':
            boardLineStatus = !boardLineStatus;
            break;
        default:
            break;
        }
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
                    PlaySecondMP3(L"sound/gun sound 2.MP3");
                    Bullet* b = nullptr;
                    if (findFalseBullet(mainBullets, b) && b != nullptr) {
                        bulletScaleDown(b, mainBlock);
                        if (!b->special)
                            b->color = RGB(0, 0, 0);
                        b->left = true;
                        b->bullet_move_status = true;
                        if (gameUi.getScore() >= 1000) {
                            if (vk_count >= 15 && vk_status) {
                                b->speed = b->max_speed;
                                /*b->upgradeBullet_l = true;
                                b->upgradeBullet_r = true;*/ //총알 3개
                            }
                        }
                    }
                    vk_count = 0;
                    vk_status = false;
                    break;
                }
                case VK_RIGHT:
                {
                    PlaySecondMP3(L"sound/gun sound 2.MP3");
                    Bullet* b = nullptr;
                    if (findFalseBullet(mainBullets, b) && b != nullptr) {
                        bulletScaleDown(b, mainBlock);
                        if (!b->special)
                            b->color = RGB(0, 0, 0);
                        b->right = true;
                        b->bullet_move_status = true;
                        if (gameUi.getScore() >= 1000) {
                            if (vk_count >= 15 && vk_status) {
                                b->speed = b->max_speed;
                            }
                        }
                    }
                    vk_count = 0;
                    vk_status = false;
                    break;
                }
                case VK_UP:
                {
                    PlaySecondMP3(L"sound/gun sound 2.MP3");
                    Bullet* b = nullptr;
                    if (findFalseBullet(mainBullets, b) && b != nullptr) {
                        bulletScaleDown(b, mainBlock);
                        if (!b->special)
                            b->color = RGB(0, 0, 0);
                        b->up = true;
                        b->bullet_move_status = true;
                        if (gameUi.getScore() >= 1000) {
                            if (vk_count >= 15 && vk_status) {
                                b->speed = b->max_speed;
                            }
                        }
                    }
                    vk_count = 0;
                    vk_status = false;
                    break;
                }
                case VK_DOWN:
                {
                    PlaySecondMP3(L"sound/gun sound 2.MP3");
                    Bullet* b = nullptr;
                    if (findFalseBullet(mainBullets, b) && b != nullptr) {
                        bulletScaleDown(b, mainBlock);
                        if (!b->special)
                            b->color = RGB(0, 0, 0);
                        b->down = true;
                        b->bullet_move_status = true;
                        if (gameUi.getScore() >= 1000) {
                            if (vk_count >= 15 && vk_status) {
                                b->speed = b->max_speed;
                            }
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
                gameStateManager.setImage(L"img/gamePlay/score bar_v2.png");
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
            redBlocks.clear();
            mainBlock.status = false;
            gameUi.setHp(300);
            gameUi.setExp(0);

            mainBullets.clear(); //총알 빈값으로
            dropBullet.clear();
            explodes.clear();

            if (gameStateManager.getLevel() == 4 || gameStateManager.getLevel() == 3) { //통과하지 못하는 special블럭 생성
                specialBlocks.clear();
                int spe_n = uid_special_block(gen);
                
                for (int i = 0; i < spe_n; ++i) {
                    uniform_int_distribution<int> uid_sp_block2(1, blocks.size());
                    Block spB;
                    spB.status = true;
                    spB.rect = blocks[uid_sp_block2(gen)].rect;
                    spB.color = RGB(0, 0, 255);
                    specialBlocks.push_back(spB);
                }
            }

            InvalidateRect(hWnd, NULL, false);
            break;
        }

        if (gameStateManager.getState() == GameState::GAMEOVER && wParam == VK_RETURN) {
            gameStateManager.setCurrentState(GameState::START);
            gameStateManager.setImage(L"img/Inversus Intro.png");
            PlayMP3(L"sound/main intro.mp3");
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

            if (boardLineStatus) {
                for (int x = 0; x <= gameUi.line_size; ++x) {
                    MoveToEx(mDC, 0, x * gameUi.cellSize + 130, NULL);
                    LineTo(mDC, rect.right, x * gameUi.cellSize + 130);
                }

                for (int y = 0; y <= gameUi.line_size; ++y) {
                    MoveToEx(mDC, y * gameUi.cellSize, 130, NULL);
                    LineTo(mDC, y * gameUi.cellSize, gameUi.gameBordRect.bottom);
                }
            }
            
            // 드랍된 총알 회전 각도 관리
            static double dropBulletAngle = 0.0;

            for (int i = 0; i < dropBullet.size(); ++i) { // drop된 총알
                dropBullet[i].bullet_drop_print(mDC, dropBullet[i], dropBulletAngle);
            }

            // dropBulletAngle을 증가시켜 총알이 계속 회전하도록 함
            dropBulletAngle += 5.0;
            if (dropBulletAngle >= 360.0) {
                dropBulletAngle -= 360.0;
            }


            gameUi.printBlackBlock(blocks, mDC);
            gameUi.drawGameUI(mDC, gameUi, rect);

            if (gameStateManager.getLevel() == 4 || gameStateManager.getLevel() == 3) { // specail block print
                for (int i = 0; i < specialBlocks.size(); ++i) {
                    if (specialBlocks[i].status) {
                        specialBlocks[i].print_special_Block(mDC, specialBlocks[i]);
                    }
                }
            }

            if (rotatingBullet.status) {
                HBRUSH hBrush = CreateSolidBrush(rotatingBullet.color);
                HBRUSH oldBrush = (HBRUSH)SelectObject(mDC, hBrush);
                Ellipse(mDC, rotatingBullet.rect.left, rotatingBullet.rect.top, rotatingBullet.rect.right, rotatingBullet.rect.bottom);
                SelectObject(mDC, oldBrush);
                DeleteObject(hBrush);
            }
            
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
            }
            else { //죽고 난 뒤, 리스폰
                mainBlock.print_main_res(mDC, mainBlock, c_n, gameUi.cellSize);
            }

            for (auto& bullet : mainBullets) { // bullet
                if (bullet.bullet_move_status) {
                    bullet.print(mDC, bullet);
                }
                else {
                    if (mainBlock.status)
                        bullet.bullet_default_print(mDC, bullet); //총알 회전
                }
            }

            // 폭발 그리기
            for (const auto& explosion : explodes) {
                explosion.draw(mDC, explosion);
                comboRect = explosion.rect;
            }

            if (combo > 1) //combo
                print_combo(mDC, combo, comboRect);

            print_score(mDC, gameUi); //score
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
                    OffsetRect(&mainBlock.rect, checkCrash(blocks, mainBlock, gameUi, specialBlocks), 0);
                }

                if (mainBlock.right) {
                    OffsetRect(&mainBlock.rect, mainBlock.speed, 0);
                    OffsetRect(&mainBlock.rect, -checkCrash(blocks, mainBlock, gameUi, specialBlocks), 0);
                }

                if (mainBlock.up) {
                    OffsetRect(&mainBlock.rect, 0, -mainBlock.speed);
                    OffsetRect(&mainBlock.rect, 0, checkCrash(blocks, mainBlock, gameUi, specialBlocks));
                }

                if (mainBlock.down) {
                    OffsetRect(&mainBlock.rect, 0, mainBlock.speed);
                    OffsetRect(&mainBlock.rect, 0, -checkCrash(blocks, mainBlock, gameUi, specialBlocks));
                }

                if (moveRedBlock(redBlocks, mainBlock, mDC, explodes, setting, specialBlocks)) { // redBlock이 mainBlock을 향해감 + main + red충돌체크

                    gameUi.setHp(gameUi.getHp() - 60);

                    if (gameUi.getHp() <= 0) {
                        //game over
                        gameStateManager.setCurrentState(GameState::GAMEOVER);
                        gameStateManager.setImage(L"img/gamePlay/game over.png");
                        CloseGameBackgroundSound();
                        PlaySecondMP3(L"sound/game over.MP3"); // 버튼 사운드
                        KillTimer(hWnd, 1);
                    }

                    mainBlock.rect = { 0,0,50,50 }; //main block 리스폰 부분
                    mainBlock.left = false;
                    mainBlock.right = false;
                    mainBlock.up = false;
                    mainBlock.down = false;
                    OffsetRect(&mainBlock.rect, (gameUi.gameBordRect.right / 2) - 25, (gameUi.gameBordRect.bottom / 2) + 30);
                    SetTimer(hWnd, 2, 100, NULL); // 죽고 난 뒤 생성 타이머
                }

                moveChangeBackgroundBlack(redBlocks, blocks); //redBlock이 지나가는 자리는 black으로 바꿈

                for (int i = mainBullets.size() - 1; i >= 0; --i) { // 총알 발사 등.
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

                        RECT r;
                        if (!IntersectRect(&r, &mainBullets[i].rect, &gameUi.gameBordRect)) { // 총알이 화면 밖으로 나가면 지우기
                            mainBullets.erase(mainBullets.begin() + i);
                            continue;
                        }

                        for (int j = specialBlocks.size() - 1; j >= 0; --j) {
                            if (mainBullets[i].special && IntersectRect(&r, &specialBlocks[j].rect, &mainBullets[i].rect)) { // 스페셜 블록 부딫히면 삭제
                                Explosion ex(specialBlocks[j].rect, specialBlocks[j].color);
                                explodes.push_back(ex);
                                specialBlocks.erase(specialBlocks.begin() + j);
                                mainBullets.erase(mainBullets.begin() + i);
                                break;
                            }
                            else if (!mainBullets[i].special && IntersectRect(&r, &specialBlocks[j].rect, &mainBullets[i].rect)) {
                                mainBullets.erase(mainBullets.begin() + i);
                                break;
                            }
                        }

                        if (mainBullets.size() > 0) {
                            if (checkRedBlockBullet(mainBullets[i], redBlocks, blocks, mDC, gameUi, explodes, combo)) { // 총알 + 빨간 블록 충돌 검사
                                // 만약 충돌된 상태면 총알 지워버리기
                                mainBullets.erase(mainBullets.begin() + i);
                            }
                        }
                    }
                }
            }

            for (auto it = explodes.begin(); it != explodes.end(); ) { // 폭발 애니메이션 프레임
                if (!it->update()) {
                    uniform_int_distribution<int> uid_drop_bullet_status(0, 1);

                    if (uid_drop_bullet_status(gen) == 0) {
                        if (explodes.size() >= 1) {
                            PlaySecondMP3(L"sound/broken sound.MP3");

                            uniform_int_distribution<int> uid_drop_bullet_num(1, 3);

                            Bullet b;
                            b.status = true;
                            b.rect = { 0,0,50,50 };
                            OffsetRect(&b.rect, it->rect.left, it->rect.top);
                            b.rect.top += 20;
                            b.rect.left += 20;
                            b.rect.right -= 20;
                            b.rect.bottom -= 20;
                            if (uid_drop_bullet_status(gen) == 0) {
                                b.color = RGB(255, 0, 0);
                                b.borderColor = RGB(255, 0, 0);
                                b.special = true;
                                b.capacity = uid_drop_bullet_num(gen);
                            }
                            else {
                                b.color = RGB(255, 255, 255);
                                b.borderColor = RGB(0, 0, 0);
                                b.capacity = uid_drop_bullet_num(gen);
                            }

                            dropBullet.push_back(b);
                        }
                    }
                    it = explodes.erase(it);
                    moveRect(gameUi, blocks, move_cnt, hWnd); // 화면 흔들림
                    gameUi.setScore(gameUi.getScore() + 100 * combo); // 기본 죽였을때 점수
                    gameUi.setExp(gameUi.getExp() + 5);
                    if (gameUi.getExp() >= 100) {
                        if (gameUi.getHp() < 300) {
                            gameUi.setHp(gameUi.getHp() + 30);
                        }
                        gameUi.setExp(0);
                    }
                }
                else {
                    ++it;
                }
            }

            gameUi.setScore(gameUi.getScore() + 1);

            {
                static double angle = 0.0;
                POINT center = { (mainBlock.rect.left + mainBlock.rect.right) / 2, (mainBlock.rect.top + mainBlock.rect.bottom) / 2 };
                rotateBullets(mainBullets, center, angle);
                angle += 5.0; // 각도 증가
                if (angle >= 360.0) {
                    angle -= 360.0;
                }
            }

            RECT ch_sp_rect;
            for (int i = 0; i < dropBullet.size(); ++i) { // 드랍된 총알 먹음
                if (IntersectRect(&ch_sp_rect, &mainBlock.rect, &dropBullet[i].rect)) {
                    int size = dropBullet[i].capacity;
                    int currentBulletCount = mainBullets.size();
                    int maxBulletsToAdd = gameUi.max_reload_cnt - currentBulletCount;

                    if (maxBulletsToAdd > 0) {
                        int bulletsToAdd = min(size, maxBulletsToAdd);
                        for (int j = 0; j < bulletsToAdd; ++j) {
                            Bullet b;
                            b.color = dropBullet[i].color;
                            b.borderColor = dropBullet[i].borderColor;
                            b.special = dropBullet[i].special;
                            b.rect = mainBlock.rect;
                            b.rect.top += 20;
                            b.rect.left += 20;
                            b.rect.right -= 20;
                            b.rect.bottom -= 20;
                            mainBullets.push_back(b);
                        }
                        dropBullet[i].capacity -= bulletsToAdd;
                        if (dropBullet[i].capacity <= 0) {
                            dropBullet.erase(dropBullet.begin() + i);
                            --i; // Adjust the index after erasing an element
                        }
                    }
                }
            }


            for (int i = redBlocks.size() - 1; i >= 0; --i) { // 위성 총알이랑 부딪히면 터짐
                RECT ch_rect;
                if (IntersectRect(&ch_rect, &redBlocks[i].rect, &rotatingBullet.rect)) {
                    // 폭발
                    Explosion b(redBlocks[i].rect, redBlocks[i].color);
                    explodes.push_back(b);
                    RECT r_ch = redBlocks[i].rect; // 없어질 검은 부분
                    InflateRect(&r_ch, gameUi.cellSize, gameUi.cellSize);
                    for (auto& block : blocks) {
                        if (IntersectRect(&ch_rect, &r_ch, &block.rect)) {
                            block.status = false;
                        }
                    }
                    redBlocks.erase(redBlocks.begin() + i);
                }
            }

            {
                static double angle1 = 0.0;
                POINT center = { (mainBlock.rect.left + mainBlock.rect.right) / 2, (mainBlock.rect.top + mainBlock.rect.bottom) / 2 };
                rotateBullets2(rotatingBullet, center, angle1);
                angle1 += 5.0; // 각도 증가
                if (angle1 >= 360.0) {
                    angle1 -= 360.0;
                }
            }

            if (!rotatingBulletStatus && gameUi.getScore() >= 10000) {
                rotatingBullet.rect = mainBlock.rect; // Initial size of the bullet
                InflateRect(&rotatingBullet.rect, -20, -20);
                rotatingBullet.color = RGB(0, 255, 0); // Yellow color for visibility
                rotatingBullet.status = true;
                rotatingBulletAngle = 0.0;
                rotatingBulletStatus = true;
            }

            if (gameUi.getScore() >= 5000) {
                gameUi.max_reload_cnt = 10;
            }

            InvalidateRect(hWnd, NULL, false);
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
                    
                    if (uid_red_rand(gen) == 0) {
                        redB.color = RGB(255, 123, 123);
                        redB.red_special = true;
                    }

                    redBlocks.push_back(redB);
                }

                

                //KillTimer(hWnd, 4);
                if (gameUi.getScore() > 10000) { //score 10000점 넘으면 빠른 스폰
                    SetTimer(hWnd, 3, 20, NULL);
                }
                else if (gameUi.getScore() > 3000) {
                    SetTimer(hWnd, 3, 80, NULL);
                }
                else if (gameUi.getScore() > 6000) {
                    SetTimer(hWnd, 3, 50, NULL);
                }
                else if (gameUi.getScore() > 8000) {
                    SetTimer(hWnd, 3, 30, NULL);
                }
                else {
                    SetTimer(hWnd, 3, 100, NULL);
                }
            }
            break;
        case 6: //press bullet
            if (vk_status) {
                vk_count++;
            }

            if (mainBullets.size() < gameUi.max_reload_cnt) { //재장전
                reloadCnt++;
                if (reloadCnt >= 15) {
                    Bullet bullet;
                    bullet.color = RGB(255, 255, 255);
                    bullet.rect = mainBlock.rect;
                    bullet.rect.top += 20;
                    bullet.rect.left += 20;
                    bullet.rect.right -= 20;
                    bullet.rect.bottom -= 20;
                    mainBullets.push_back(bullet);
                    reloadCnt = 0;
                }
            }

            break;
        case 7:
            if (move_cnt > 0) {
                static std::random_device rd;
                static std::mt19937 gen(rd());
                static std::uniform_int_distribution<int> dist(-SHAKE_MAGNITUDE, SHAKE_MAGNITUDE);

                // 무작위 오프셋 적용
                int offsetX = dist(gen);
                int offsetY = dist(gen);

                // 게임 보드 흔들림 적용
                gameUi.gameBordRect = gameUi.originalGameBordRect; // 원래 위치로 리셋
                OffsetRect(&gameUi.gameBordRect, offsetX, offsetY);

                // 각 블록에 흔들림 적용
                for (auto& block : blocks) {
                    block.rect = block.originalRect; // 원래 위치로 리셋
                    OffsetRect(&block.rect, offsetX, offsetY);
                }

                --move_cnt;

                InvalidateRect(hWnd, NULL, false); // 화면 갱신 요청
            }
            else {
                KillTimer(hWnd, SHAKE_TIMER); // 타이머 중지
                gameUi.gameBordRect = gameUi.originalGameBordRect; // 원래 위치로 리셋

                // 각 블록 원래 위치로 리셋
                for (auto& block : blocks) {
                    block.rect = block.originalRect;
                }

                InvalidateRect(hWnd, NULL, false); // 최종 화면 갱신 요청
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