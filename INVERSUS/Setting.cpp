#include <Windows.h>
#include<string>

#include "sound.h"
#include "Setting.h"

void Setting::setting(WPARAM wParam, HWND& hWnd) {
    PlaySecondMP3(L"sound/button sound.MP3"); // 버튼 사운드
    const std::wstring PATH = L"img/setting/";
    switch (wParam)
    {
    case VK_DOWN:

        if (se_n == 3) {
            se_n = 1;
        }
        else {
            se_n++;
        }

        break;
    case VK_UP:
        if (se_n == 1) {
            se_n = 3;
        }
        else {
            se_n--;
        }
        break;
    case VK_LEFT:
        if (music_n == 0) {
            music_n = 3;
        }
        else {
            music_n--;
        }
        break;
    case VK_RIGHT:
        if (music_n == 2) {
            music_n = 0;
        }
        else {
            music_n++;
        }
        break;
    default:
        break;
    }

    // -------- 이미지 넣음
    switch (se_n)
    {
    case 1:
        gameStateManager->setImage((PATH + L"setting_default.png").c_str());
        break;
    case 2:
        gameStateManager->setImage((PATH + L"setting_select.png").c_str());
        break;
    case 3:
        gameStateManager->setImage((PATH + L"setting_close.png").c_str());
        break;
    default:
        break;
    }

    if (VK_RETURN == wParam) {
        switch (se_n) {
        case 1:
           
            break;
        case 2:
          
            break;
        case 3: // 뒤로가기
            KillTimer(hWnd, 1);
            CloseGameBackgroundSound();
            gameStateManager->setImage(L"img/Inversus Intro.png");
            PlayMP3(L"sound/main intro.mp3");
            gameStateManager->setCurrentState(GameState::START);
            break;
        default:
            break;
        }
    }


};