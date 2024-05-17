#pragma once
#include <Windows.h>

class Bullet {
private:
public:
	//int capacity; //총알 수
	RECT rect;
	COLORREF color = RGB(0, 0, 0);
	int speed = 1;

	bool status = false; // 발사 상태

	bool left = false;
	bool right = false;
	bool up = false;
	bool down = false;

	void print(HDC& mDC, Bullet& bullet) {
		// 투명 브러시 선택
		HBRUSH hBrush = CreateSolidBrush(bullet.color);
		HBRUSH oldBrush = (HBRUSH)SelectObject(mDC, hBrush);

		RoundRect(mDC, bullet.rect.left, bullet.rect.top, bullet.rect.right, bullet.rect.bottom, 20, 20);

		// 원래 펜으로 복구
		SelectObject(mDC, oldBrush);
		DeleteObject(hBrush);

	}
};