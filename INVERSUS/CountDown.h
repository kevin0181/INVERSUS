#include"GameUI.h"
#include "sound.h"

void setCountDown(GameUI& gameUi, HWND& hWnd) {
    gameUi.countDownStatus = true;
    if (gameUi.countDown == 3) {
        gameUi.countDownStatus = false;
        KillTimer(hWnd, 10);
        GameBackgroundSound(0);
    }
    gameUi.countDown++;
}