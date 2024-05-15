#include"GameUI.h"
#include "sound.h"

void setCountDown(GameUI& gameUi, HWND& hWnd, Block& mainBlock) {
    gameUi.countDownStatus = true;
    if (gameUi.countDown == 3) {
        gameUi.countDownStatus = false;
        gameUi.countDown = 0;
        KillTimer(hWnd, 10);
        KillTimer(hWnd, 2);
        mainBlock.status = true;
        SetTimer(hWnd, 1, 1, NULL); //game play timer
        GameBackgroundSound(setting.getMusic_n());

    }
    gameUi.countDown++;
}