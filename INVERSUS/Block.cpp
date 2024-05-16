#include "Block.h"

void Block::print_red_Block(HDC& mDC, Block& redBlock) {
	HBRUSH hBrush;
	HBRUSH oldBrush;

	SetBkMode(mDC, TRANSPARENT);

	hBrush = CreateHatchBrush(HS_DIAGCROSS, RGB(255, 0, 0));
	oldBrush = (HBRUSH)SelectObject(mDC, hBrush);

	RoundRect(mDC, redBlock.aroundRect.left, aroundRect.top, aroundRect.right, aroundRect.bottom, 20, 20);

	SelectObject(mDC, oldBrush);
	DeleteObject(hBrush);

	hBrush = CreateSolidBrush(redBlock.color);
	oldBrush = (HBRUSH)SelectObject(mDC, hBrush);

	RoundRect(mDC, redBlock.rect.left, redBlock.rect.top, redBlock.rect.right, redBlock.rect.bottom, 20, 20);

	SelectObject(mDC, oldBrush);
	DeleteObject(hBrush);
}

void Block::print_red_res(HDC& mDC, Block& redBlock, int r_n, int cellSize) {

	int maxShrink = 30;  // 최대 축소 크기
	int shrink = (maxShrink * r_n) / 12;  // r_n에 따른 축소량 계산

	// 사각형의 새로운 크기 계산
	RECT tempRect = {
		redBlock.rect.left + shrink,
		redBlock.rect.top + shrink,
		redBlock.rect.right - shrink,
		redBlock.rect.bottom - shrink
	};

	// 투명 브러시 선택
	HBRUSH hBrush = (HBRUSH)GetStockObject(NULL_BRUSH);
	HBRUSH oldBrush = (HBRUSH)SelectObject(mDC, hBrush);

	// 펜 생성 및 선택
	HPEN hPen = CreatePen(PS_SOLID, 2, redBlock.color); // 빨간색, 굵기 3의 펜
	HGDIOBJ hOldPen = SelectObject(mDC, hPen);

	RoundRect(mDC, tempRect.left, tempRect.top, tempRect.right, tempRect.bottom, 20, 20);

	// 원래 펜으로 복구
	SelectObject(mDC, hOldPen);
	DeleteObject(hPen);
	SelectObject(mDC, oldBrush);

};
