#include "GameUI.h"

void GameUI::drawGameUI(HDC& mDC, GameUI& gameUi) {
	
	if (gameUi.countDownStatus) { //count down
		HBRUSH hBrush = CreateSolidBrush(RGB(200, 200, 200));
		HBRUSH oldBrush = (HBRUSH)SelectObject(mDC, hBrush);
		Rectangle(mDC, 0, 0, 500, 500);
		SelectObject(mDC, oldBrush);
		DeleteObject(hBrush);
	}

}

