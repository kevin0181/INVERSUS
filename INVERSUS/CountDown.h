#include"GameUI.h"

void setCountDown(GameUI& gameUi, HWND& hWnd) {
    gameUi.countDownStatus = true;
    if (gameUi.countDown == 3) {
        gameUi.countDownStatus = false;
        KillTimer(hWnd, 10);
    }
    gameUi.countDown++;
}