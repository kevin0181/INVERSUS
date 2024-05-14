#include "LevelSetting.h"
#include "sound.h"

void LevelSetting::level_setting(WPARAM wParam, HWND& hWnd) {
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
            PlayMP3Close();
            CloseSecondMP3();
            gameStateManager->setLevel(level);
            gameStateManager->setImage(L"img/gamePlay/score bar.png");
            gameStateManager->setCurrentState(GameState::GAMEPLAY);
            PlaySecondMP3(L"sound/countdown2.mp3"); // 버튼 사운드
            SetTimer(hWnd, 10, 1000, NULL);  // count down start
            break;
        case 2: // level nomal
            PlayMP3Close();
            CloseSecondMP3();
            gameStateManager->setLevel(level);
            gameStateManager->setImage(L"img/gamePlay/score bar.png");
            gameStateManager->setCurrentState(GameState::GAMEPLAY);
            PlaySecondMP3(L"sound/countdown2.wav"); // 버튼 사운드
            SetTimer(hWnd, 10, 1000, NULL);  // count down start
            break;
        case 3: // level hard
            PlayMP3Close();
            CloseSecondMP3();
            gameStateManager->setLevel(level);
            gameStateManager->setImage(L"img/gamePlay/score bar.png");
            gameStateManager->setCurrentState(GameState::GAMEPLAY);
            PlaySecondMP3(L"sound/countdown2.wav"); // 버튼 사운드
            SetTimer(hWnd, 10, 1000, NULL);  // count down start
            break;
        case 4: // level very hard
            PlayMP3Close();
            CloseSecondMP3();
            gameStateManager->setLevel(level);
            gameStateManager->setImage(L"img/gamePlay/score bar.png");
            gameStateManager->setCurrentState(GameState::GAMEPLAY);
            PlaySecondMP3(L"sound/countdown2.wav"); // 버튼 사운드
            SetTimer(hWnd, 10, 1000, NULL);  // count down start
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