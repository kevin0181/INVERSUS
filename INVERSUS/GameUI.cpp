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
	drawHP(mDC, rect, gameUi.hp, gameUi);
	
	// exp bar
	drawExp(mDC, rect, gameUi.exp);

	// score
	
}

void GameUI::drawHP(HDC& mDC, const RECT& rect, int hp, GameUI& gameUi) {

	HBRUSH hBrush;
	HBRUSH oldBrush;

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
}


void GameUI::drawExp(HDC& mDC, const RECT& rect, int exp) {

	// exp의 비율을 계산 (0 ~ 1)
	float expRatio = static_cast<float>(exp) / 100.0f;
	// 비율에 따른 가로 길이 계산
	int barWidth = static_cast<int>(rect.right * expRatio);
	// exp 바의 RECT 설정
	RECT expRect = { 0, gameBordRect.bottom, barWidth, gameBordRect.bottom + 10 };  // 높이는 임의로 20으로 설정

	// 배경 브러시 설정
	HBRUSH hBrush = CreateSolidBrush(RGB(0, 255, 0));  // 녹색 바
	HBRUSH oldBrush = (HBRUSH)SelectObject(mDC, hBrush);

	// 실제 바 그리기
	FillRect(mDC, &expRect, hBrush);

	// 오브젝트 정리
	SelectObject(mDC, oldBrush);
	DeleteObject(hBrush);
}
