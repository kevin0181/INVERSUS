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
            music_n = 2;
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

void Setting::draw_setting(HDC& mDC, RECT rect) {
    // Set text color to white and background mode to transparent
    SetTextColor(mDC, RGB(255, 255, 255));
    SetBkMode(mDC, TRANSPARENT);

    // Create a bold font
    HFONT hFont = CreateFont(24, 0, 0, 0, FW_BOLD, FALSE, FALSE, FALSE, DEFAULT_CHARSET,
        OUT_OUTLINE_PRECIS, CLIP_DEFAULT_PRECIS, CLEARTYPE_QUALITY,
        VARIABLE_PITCH, TEXT("Arial"));

    SelectObject(mDC, hFont);

    TextOut(mDC, rect.right / 2 - 110, rect.bottom / 2 - 10, fileName[music_n].c_str(), fileName[music_n].size());

    const std::wstring invi_status1 = L"무적모드 켜기";
    const std::wstring invi_status2 = L"무적모드 끄기";

    SetTextColor(mDC, RGB(0, 0, 0));

    if (invincibility) {
        TextOut(mDC, rect.right / 2 - 60, rect.bottom / 2 + 30, invi_status2.c_str(), invi_status2.size());
    }
    else {
        TextOut(mDC, rect.right / 2 - 60, rect.bottom / 2 + 30, invi_status1.c_str(), invi_status1.size());
    }

    DeleteObject(hFont);
}