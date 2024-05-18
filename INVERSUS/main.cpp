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

    // ��ü ȭ�� ũ�⸦ �����ɴϴ�.
    int screenWidth = GetSystemMetrics(SM_CXSCREEN);
    int screenHeight = GetSystemMetrics(SM_CYSCREEN);

    // ��üȭ�� ��Ÿ�Ϸ� â�� �����մϴ�.
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
    static int reloadCnt = 0; // ������ ��Ÿ�� 1�߾�.

    static int c_n = 0; //response �̹���
    static int r_n = 0;
  
    static int combo = 0;
    static RECT comboRect;

    static vector<Block> redBlocks;
    static vector<Block> specialBlocks;

    static vector<Explosion> explodes;

    static int move_cnt = 0;
    static bool boardLineStatus = false;

    switch (uMsg)
    {
    case WM_CREATE:
    {   
        GetClientRect(hWnd, &rect);
        gameStateManager.setCurrentState(GameState::START);
        gameStateManager.setImage(L"img/Inversus Intro.png");
        PlayMP3(L"sound/main intro.mp3");

        SetTimer(hWnd, 6, 100, NULL); // press bullet timer

        break;
    }
    case WM_CHAR:
        switch (wParam)
        {
        case '[':
            backgroundS -= 100;
            if (backgroundS < -10000) backgroundS = -10000; // �ּ� ���� ����
            SetVolume(backgroundS);
            break;
        case ']':
            backgroundS += 100;
            if (backgroundS > 0) backgroundS = 0; // �ִ� ���� ����
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
    case WM_KEYDOWN:  // Ű���� Ű�� ������ ��

        if (wParam == VK_ESCAPE) { //esc -> ����
            if (gameStateManager.getState() == GameState::GAMEPLAY) { // game play -> setting
                if (gameUi.countDown != 1) // ������ ���۵Ǹ� �����ϰ�
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

        if (gameStateManager.getState() == GameState::SETTING) { //setting �����϶� ������
            setting.setting(wParam, hWnd);
            InvalidateRect(hWnd, NULL, false);
            break;
        }

        if (gameStateManager.getState() == GameState::START && wParam == VK_RETURN) { // ����ȭ�� -> player Select ȭ��
            PlaySecondMP3(L"sound/button sound.MP3"); // ��ư ����
            gameStateManager.setImage(L"img/player/player_0_v2.png");
            gameStateManager.setCurrentState(GameState::PLAYER);
            InvalidateRect(hWnd, NULL, false);
            break;
        }

        if (gameStateManager.getState() == GameState::PLAYER) { // ���� ���� �� setting
            playerSetting.game_setting(wParam);
            InvalidateRect(hWnd, NULL, false);
            break;
        }

        if (gameStateManager.getState() == GameState::LEVEL) { // game level ����
            levelSetting.level_setting(wParam, hWnd, rect, mainBlock, blocks);
            gameUi.setBlackBlock(blocks, gameUi.cellSize); // ���� �� ����
            redBlocks.clear();
            mainBlock.status = false;
            gameUi.setHp(300);
            gameUi.setExp(0);

            mainBullets.clear(); //�Ѿ� ������

            if (gameStateManager.getLevel() == 4) { //������� ���ϴ� special�� ����
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

        gameStateManager.DrawImage(mDC, rect); // ��ü���

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
            
            gameUi.printBlackBlock(blocks, mDC);
            gameUi.drawGameUI(mDC, gameUi, rect);

            if (gameStateManager.getLevel() == 4) { // specail block print
                for (int i = 0; i < specialBlocks.size(); ++i) {
                    if (specialBlocks[i].status) {
                        specialBlocks[i].print_special_Block(mDC, specialBlocks[i]);
                    }
                }
            }
            
            for (auto& redB : redBlocks) {
                if (redB.status) {
                    redB.print_red_Block(mDC, redB);
                }
                else {
                    redB.print_red_res(mDC, redB, r_n, gameUi.cellSize);
                }
            }

            if (mainBlock.status) { //��� ���� ���
                gameUi.mainAsset(mDC, mainBlock);
            }
            else { //�װ� �� ��, ������
                mainBlock.print_main_res(mDC, mainBlock, c_n, gameUi.cellSize);
            }

            for (auto& bullet : mainBullets) { // bullet
                if (bullet.bullet_move_status) {
                    bullet.print(mDC, bullet);
                }
                else {
                    if (mainBlock.status)
                        bullet.bullet_default_print(mDC, bullet); //�Ѿ� ȸ��
                }
            }

            // ���� �׸���
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

        DeleteObject(hBitmap); // ������ ��Ʈ�� ����
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

                if (moveRedBlock(redBlocks, mainBlock, mDC, explodes, setting)) { // redBlock�� mainBlock�� ���ذ� + main + red�浹üũ

                    gameUi.setHp(gameUi.getHp() - 30);

                    if (gameUi.getHp() <= 0) {
                        //game over

                    }

                    mainBlock.rect = { 0,0,50,50 }; //main block ������ �κ�
                    mainBlock.left = false;
                    mainBlock.right = false;
                    mainBlock.up = false;
                    mainBlock.down = false;
                    OffsetRect(&mainBlock.rect, (gameUi.gameBordRect.right / 2) - 25, (gameUi.gameBordRect.bottom / 2) + 30);
                    SetTimer(hWnd, 2, 100, NULL); // �װ� �� �� ���� Ÿ�̸�
                }
                
                moveChangeBackgroundBlack(redBlocks, blocks); //redBlock�� �������� �ڸ��� black���� �ٲ�

                for (int i = 0; i < mainBullets.size(); ++i) { // �Ѿ� �߻� ��.
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

                        checkBulletBlock(mainBullets[i], blocks); // �Ѿ� + ���� ��� �浹 �˻�

                        RECT r;
                        if (!IntersectRect(&r, &mainBullets[i].rect, &gameUi.gameBordRect)) {
                            mainBullets.erase(mainBullets.begin() + i);
                        }

                        if (mainBullets.size() > 0) {
                            if (checkRedBlockBullet(mainBullets[i], redBlocks, blocks, mDC, gameUi, explodes, combo)) { // �Ѿ� + ���� ��� �浹 �˻�
                                //���� �浹�� ���¸� �Ѿ� ����������
                                mainBullets.erase(mainBullets.begin() + i);
                            }
                        }
                    }
                }
            }

            for (auto it = explodes.begin(); it != explodes.end(); ) { // ���� �ִϸ��̼� ������
                if (!it->update()) {
                    it = explodes.erase(it);
                    moveRect(gameUi, blocks, move_cnt, hWnd);
                    gameUi.setScore(gameUi.getScore() + 100 * combo); // �⺻ �׿����� ����
                    gameUi.setExp(gameUi.getExp() + 5);
                    if (gameUi.getExp() >= 100) {
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
                angle += 5.0; // ���� ����
                if (angle >= 360.0) {
                    angle -= 360.0;
                }
            }

            InvalidateRect(hWnd, NULL, false);
            break;
        case 2: // main block resp Ÿ�̸�
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
                blankMain(blocks, &mainBlock, redBlocks); // �� �κ� �����
                KillTimer(hWnd, 2);
                break;
            }

            InvalidateRect(hWnd, NULL, false);
            break;
        }
        case 3: //red block resp Ÿ�̸�
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
        case 4: //red block ���� Ÿ�̸�
            if (mainBlock.status && redBlocks.size() <= 0) { // ���� ���� Ȱ��ȭ �Ǿ� �ִ� ���¿��� redBlock ����
                uniform_int_distribution<int> uid_redBlock(1, gameStateManager.getLevel() * 2 + 2);
                uniform_int_distribution<int> uid_redBlockRect(1, blocks.size());

                int rand = uid_redBlock(gen); // redBlock ����
                vector<int> rand_i; //redBlock ��
                set<int> uniqueNumbers; // �ߺ� ������ ���� ����

                while (rand_i.size() < rand) {
                    int num = uid_redBlockRect(gen);

                    // �ߺ� ���� �˻�
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
                //KillTimer(hWnd, 4);
                SetTimer(hWnd, 3, 100, NULL);
            }
            break;
        case 6: //press bullet
            if (vk_status) {
                vk_count++;
            }

            if (mainBullets.size() < 6) { //������
                reloadCnt++;
                if (reloadCnt >= 15) {
                    Bullet bullet;
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

                // ������ ������ ����
                int offsetX = dist(gen);
                int offsetY = dist(gen);

                // ���� ���� ��鸲 ����
                gameUi.gameBordRect = gameUi.originalGameBordRect; // ���� ��ġ�� ����
                OffsetRect(&gameUi.gameBordRect, offsetX, offsetY);

                // �� ��Ͽ� ��鸲 ����
                for (auto& block : blocks) {
                    block.rect = block.originalRect; // ���� ��ġ�� ����
                    OffsetRect(&block.rect, offsetX, offsetY);
                }

                --move_cnt;

                InvalidateRect(hWnd, NULL, false); // ȭ�� ���� ��û
            }
            else {
                KillTimer(hWnd, SHAKE_TIMER); // Ÿ�̸� ����
                gameUi.gameBordRect = gameUi.originalGameBordRect; // ���� ��ġ�� ����

                // �� ��� ���� ��ġ�� ����
                for (auto& block : blocks) {
                    block.rect = block.originalRect;
                }

                InvalidateRect(hWnd, NULL, false); // ���� ȭ�� ���� ��û
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