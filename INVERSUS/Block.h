#pragma once
#include <Windows.h>

class Block {
public:

	RECT rect;
	COLORREF borderColor;
	COLORREF color;
	bool status;

	Block(COLORREF color, COLORREF borderColor, RECT rect) :rect(rect), status(false), color(color), borderColor(borderColor) {};
	void print_Block(HDC& mDC, int cellSize);
};