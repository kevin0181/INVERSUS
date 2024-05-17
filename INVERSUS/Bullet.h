#pragma once
#include <Windows.h>

class Bullet {
private:
public:
	//int capacity; //�Ѿ� ��
	RECT rect;
	COLORREF color = RGB(0, 0, 0);
	int speed = 1;

	bool status = false; // �߻� ����

	bool left = false;
	bool right = false;
	bool up = false;
	bool down = false;

	void print(HDC& mDC, Bullet& bullet) {
		// ���� �귯�� ����
		HBRUSH hBrush = CreateSolidBrush(bullet.color);
		HBRUSH oldBrush = (HBRUSH)SelectObject(mDC, hBrush);

		RoundRect(mDC, bullet.rect.left, bullet.rect.top, bullet.rect.right, bullet.rect.bottom, 20, 20);

		// ���� ������ ����
		SelectObject(mDC, oldBrush);
		DeleteObject(hBrush);

	}
};