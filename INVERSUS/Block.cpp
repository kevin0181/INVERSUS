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

	int maxShrink = 30;  // �ִ� ��� ũ��
	int shrink = (maxShrink * r_n) / 12;  // r_n�� ���� ��ҷ� ���

	// �簢���� ���ο� ũ�� ���
	RECT tempRect = {
		redBlock.rect.left + shrink,
		redBlock.rect.top + shrink,
		redBlock.rect.right - shrink,
		redBlock.rect.bottom - shrink
	};

	// ���� �귯�� ����
	HBRUSH hBrush = (HBRUSH)GetStockObject(NULL_BRUSH);
	HBRUSH oldBrush = (HBRUSH)SelectObject(mDC, hBrush);

	// �� ���� �� ����
	HPEN hPen = CreatePen(PS_SOLID, 2, redBlock.color); // ������, ���� 3�� ��
	HGDIOBJ hOldPen = SelectObject(mDC, hPen);

	RoundRect(mDC, tempRect.left, tempRect.top, tempRect.right, tempRect.bottom, 20, 20);

	// ���� ������ ����
	SelectObject(mDC, hOldPen);
	DeleteObject(hPen);
	SelectObject(mDC, oldBrush);

};
