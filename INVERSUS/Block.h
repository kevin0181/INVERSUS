#pragma once
#include <Windows.h>
#include"Bullet.h"
class Block {
public:
	
	RECT rect;
	COLORREF borderColor;
	COLORREF color;
	bool status;
	Bullet bullet;

	Block(COLORREF color, COLORREF borderColor, RECT rect) :rect(rect), status(true), color(color), borderColor(borderColor) {};

	Block(COLORREF color, COLORREF borderColor, RECT rect, bool status) : rect(rect), status(status), color(color), borderColor(borderColor) {};

	void print_Block(HDC& mDC, int cellSize);

	void setRect(RECT rect) {
		this->rect = rect;
	}
};