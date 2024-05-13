#include "GameUI.h"

void GameUI::drawGameUI(HDC& mDC, GameUI& gameUi, RECT rect) {
	HBRUSH hBrush;
	HBRUSH oldBrush;
	// main asset
	if (gameUi.countDownStatus) { //count down
		hBrush = CreateSolidBrush(RGB(200, 200, 200));
		oldBrush = (HBRUSH)SelectObject(mDC, hBrush);
		Rectangle(mDC, 0, 0, 500, 500);
		SelectObject(mDC, oldBrush);
		DeleteObject(hBrush);
	}

	// hp bar
	CImage hpImage;
	hpImage.Load(L"img/gamePlay/health bar_v2.png");

	RECT hpRect;
	hpRect.left = rect.right - hpImage.GetWidth();
	hpRect.top = rect.top + 40;
	hpRect.right = hpRect.left + gameUi.getHp();
	hpRect.bottom = hpRect.top + 33;
	hBrush = CreateSolidBrush(RGB(255, 0, 0));

	FillRect(mDC, &hpRect, hBrush);

	DeleteObject(hBrush);

	hpImage.Draw(mDC, rect.right - hpImage.GetWidth() - 30, rect.top + 30, hpImage.GetWidth(), hpImage.GetHeight(), 0, 0, hpImage.GetWidth(), hpImage.GetHeight());
	

	// exp bar

	// score
	
}

