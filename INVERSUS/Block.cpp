#include "Block.h"

void Block::print_red_Block(HDC& mDC,const Block& redBlock) {
	HBRUSH hBrush;
	HBRUSH oldBrush;

	SetBkMode(mDC, TRANSPARENT);

	hBrush = CreateHatchBrush(HS_DIAGCROSS, redBlock.color);
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

void Block::print_special_Block(HDC& mDC, const Block& block) {
	HBRUSH hBrush;
	HBRUSH oldBrush;

	hBrush = CreateSolidBrush(block.color);
	oldBrush = (HBRUSH)SelectObject(mDC, hBrush);

	Rectangle(mDC, block.rect.left, block.rect.top, block.rect.right, block.rect.bottom);

	SelectObject(mDC, oldBrush);
	DeleteObject(hBrush);
}

void Block::print_red_res(HDC& mDC, const Block& redBlock, const int& r_n, const int& cellSize) {

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


void Block::print_main_res(HDC& mDC, const Block& mainBlock, const int& r_n, const int& cellSize) {

	int maxShrink = 30;  // �ִ� ��� ũ��
	int shrink = (maxShrink * r_n) / 12;  // r_n�� ���� ��ҷ� ���

	// �簢���� ���ο� ũ�� ���
	RECT tempRect = {
		mainBlock.rect.left + shrink,
		mainBlock.rect.top + shrink,
		mainBlock.rect.right - shrink,
		mainBlock.rect.bottom - shrink
	};

	// ���� �귯�� ����
	HBRUSH hBrush = (HBRUSH)GetStockObject(NULL_BRUSH);
	HBRUSH oldBrush = (HBRUSH)SelectObject(mDC, hBrush);

	// �� ���� �� ����
	HPEN hPen;
	if (r_n % 2 == 0)
		hPen = CreatePen(PS_SOLID, 3, RGB(255, 0, 0)); // ������, ���� 3�� ��
	else
		hPen = CreatePen(PS_SOLID, 3, RGB(200, 200, 200)); // ������, ���� 3�� ��

	HGDIOBJ hOldPen = SelectObject(mDC, hPen);

	RoundRect(mDC, tempRect.left, tempRect.top, tempRect.right, tempRect.bottom, 20, 20);

	// ���� ������ ����
	SelectObject(mDC, hOldPen);
	DeleteObject(hPen);
	SelectObject(mDC, oldBrush);

};