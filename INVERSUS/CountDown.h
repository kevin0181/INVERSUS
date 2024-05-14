#include"GameUI.h"
#include "sound.h"

void setCountDown(GameUI& gameUi, HWND& hWnd) {
    gameUi.countDownStatus = true;
    if (gameUi.countDown == 3) {
        gameUi.countDownStatus = false;
        KillTimer(hWnd, 10);
        SetTimer(hWnd, 1, 1, NULL); //game play timer
        GameBackgroundSound(setting.getMusic_n());
    }
    gameUi.countDown++;
}