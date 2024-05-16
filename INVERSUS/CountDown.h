#include"GameUI.h"
#include "sound.h"

void setCountDown(GameUI& gameUi, HWND& hWnd, Block& mainBlock) {
    gameUi.countDownStatus = true;
    if (gameUi.countDown == 2) {
        gameUi.countDownStatus = false;
        gameUi.countDown = 0;
        KillTimer(hWnd, 10);
        mainBlock.status = true;
        SetTimer(hWnd, 1, 1, NULL); //game play timer
        GameBackgroundSound(setting.getMusic_n());

    }
    gameUi.countDown++;
}