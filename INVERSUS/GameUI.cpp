#include "GameUI.h"

void GameUI::drawGameUI(HDC& mDC, GameUI& gameUi, RECT rect) {

	// main asset
	if (gameUi.countDownStatus) { //count down
		//mainAsset(mDC, rect, gameUi);
	}

	// hp bar
	drawHP(mDC, rect, gameUi.hp, gameUi);
	
	// exp bar
	drawExp(mDC, rect, gameUi.exp);

	// score
	
}

void GameUI::printBlackBlock(std::vector<Block> blocks, HDC& mDC) { //���� ��� ���

	for (int i = 0; i < blocks.size(); ++i) {
		// ������ �귯���� ȸ�� �� ����
		HBRUSH hBrush = CreateSolidBrush(blocks[i].color);
		HPEN hPen = CreatePen(PS_SOLID, 1, blocks[i].borderColor);
		// ���� ��� �귯���� ����
		HBRUSH oldBrush = (HBRUSH)SelectObject(mDC, hBrush);
		HPEN oldPen = (HPEN)SelectObject(mDC, hPen);
		Rectangle(mDC, blocks[i].rect.left, blocks[i].rect.top, blocks[i].rect.right, blocks[i].rect.bottom);
		// ���� ��� �귯���� ����
		SelectObject(mDC, oldBrush);
		SelectObject(mDC, oldPen);

		// �귯���� �� �ڿ� ����
		DeleteObject(hBrush);
		DeleteObject(hPen);
	}

}

void GameUI::setBlackBlock(std::vector<Block>& blocks, int cellSize) { // ���� �� ���� ũ�⿡ ���缭 ����

	for (int y = gameBordRect.top; y < gameBordRect.bottom; y += cellSize) {
		for (int x = gameBordRect.left; x < gameBordRect.right; x += cellSize) {
			RECT blockRect = { x, y, x + cellSize, y + cellSize };
			Block b(RGB(0, 0, 0), RGB(125, 125, 125), blockRect);
			blocks.push_back(b);
		}
	}
	
}

void GameUI::mainAsset(HDC& mDC, const RECT& rect, Block mainBlock) { 

	HBRUSH hBrush;
	HBRUSH oldBrush;

	hBrush = CreateSolidBrush(mainBlock.color);
	oldBrush = (HBRUSH)SelectObject(mDC, hBrush);

	RoundRect(mDC, mainBlock.rect.left, mainBlock.rect.top, mainBlock.rect.right, mainBlock.rect.bottom, 20, 20);

	SelectObject(mDC, oldBrush);
	DeleteObject(hBrush);

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

	// exp�� ������ ��� (0 ~ 1)
	float expRatio = static_cast<float>(exp) / 100.0f;
	// ������ ���� ���� ���� ���
	int barWidth = static_cast<int>(rect.right * expRatio);
	// exp ���� RECT ����
	RECT expRect = { 0, gameBordRect.bottom, barWidth, gameBordRect.bottom + 10 };  // ���̴� ���Ƿ� 20���� ����

	// ��� �귯�� ����
	HBRUSH hBrush = CreateSolidBrush(RGB(0, 255, 0));  // ��� ��
	HBRUSH oldBrush = (HBRUSH)SelectObject(mDC, hBrush);

	// ���� �� �׸���
	FillRect(mDC, &expRect, hBrush);

	// ������Ʈ ����
	SelectObject(mDC, oldBrush);
	DeleteObject(hBrush);
}
