#include <vector>

#include "LevelSetting.h"
#include "sound.h"
#include "Global.h"

void LevelSetting::level_setting(WPARAM wParam, HWND& hWnd, RECT rect, Block& mainBlock, std::vector<Block>& blocks) {
    PlaySecondMP3(L"sound/button sound.MP3"); // 버튼 사운드
    const std::wstring PATH = L"img/level/";
    switch (wParam)
    {
    case VK_DOWN:

        if (level == 5) {
            level = 1;
        }
        else {
            level++;
        }

        break;
    case VK_UP:
        if (level == 1) {
            level = 5;
        }
        else {
            level--;
        }

        if (level == -1) {
            level = 5;
        }

        break;
    default:
        break;
    }

    // -------- 이미지 넣음
    switch (level)
    {
    case 1:
        gameStateManager->setImage((PATH + L"easy_v2.png").c_str());
        break;
    case 2:
        gameStateManager->setImage((PATH + L"nomal_v2.png").c_str());
        break;
    case 3:
        gameStateManager->setImage((PATH + L"hard_v2.png").c_str());
        break;
    case 4:
        gameStateManager->setImage((PATH + L"very hard_v2.png").c_str());
        break;
    case 5:
        gameStateManager->setImage((PATH + L"back_v2.png").c_str());
        break;
    default:
        break;

    }

    if (VK_RETURN == wParam) {
        switch (level) {
        case 1: // lelvel easy
            setEnter(hWnd, rect, mainBlock, blocks);
            break;
        case 2: // level nomal
            setEnter(hWnd, rect, mainBlock, blocks);

            break;
        case 3: // level hard
            setEnter(hWnd, rect, mainBlock, blocks);

            break;
        case 4: // level very hard
            setEnter(hWnd, rect, mainBlock, blocks);
            break;
        case 5: // 뒤로가기
            if (gameStateManager->getPlayer() == 1) {
                gameStateManager->setImage(L"img/player/player_1_v2.png");
            }
            else if (gameStateManager->getPlayer() == 2) {
                gameStateManager->setImage(L"img/player/player_2_v2.png");
            }
            gameStateManager->setCurrentState(GameState::PLAYER);
            break;
        default:
            break;
        }
    }
}

void LevelSetting::gameBoard(RECT rect) { //게임 보드 크기, cellSize, gameBord구하기

    gameUi->cellSize = (rect.right) / gameUi->line_size;

    RECT gameBord = rect;
    gameBord.top += 130;

    for (int x = 0; x <= gameUi->line_size; ++x) {
        if (x * gameUi->cellSize + 130 > rect.bottom) {
            gameBord.bottom = (x - 1) * gameUi->cellSize + 130;
            break;
        }
    }

    gameUi->gameBordRect = gameBord;
}

void LevelSetting::setEnter(HWND hWnd, RECT rect, Block& mainBlock, std::vector<Block>& blocks) {
    mainBlock.rect = { 0,0,50,50 };
    PlayMP3Close();
    CloseSecondMP3();
    gameStateManager->setLevel(level);
    gameStateManager->setImage(L"img/gamePlay/score bar.png");
    gameStateManager->setCurrentState(GameState::GAMEPLAY);
    PlaySecondMP3(L"sound/countdown2.wav"); // 버튼 사운드
    SetTimer(hWnd, 10, 1000, NULL);  // count down start
    gameUi->line_size = (gameStateManager->getLevel() * 10);
    gameBoard(rect);
    OffsetRect(&mainBlock.rect, (gameUi->gameBordRect.right / 2) - 25, (gameUi->gameBordRect.bottom / 2) + 30);
}