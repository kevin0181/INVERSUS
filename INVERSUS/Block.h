#pragma once

#include<atlimage.h>
#include <Windows.h>
#include <string>
#include"Bullet.h"

class Block {
public:
	
	RECT rect;
	RECT aroundRect;

	COLORREF borderColor;
	COLORREF color;
	bool status;
	Bullet bullet;

	bool left = false;
	bool right = false;
	bool up = false;
	bool down = false;

	CImage respImg;

	std::wstring mainRespW[12] = {
		  L"img/gamePlay/spawn/1.png",
		  L"img/gamePlay/spawn/2.png",
		  L"img/gamePlay/spawn/3.png",
		  L"img/gamePlay/spawn/4.png",
		  L"img/gamePlay/spawn/5.png",
		  L"img/gamePlay/spawn/6.png",
		  L"img/gamePlay/spawn/7.png",
		  L"img/gamePlay/spawn/8.png",
		  L"img/gamePlay/spawn/9.png",
		  L"img/gamePlay/spawn/10.png",
		  L"img/gamePlay/spawn/11.png",
		  L"img/gamePlay/spawn/12.png",
	};

	std::wstring redRespW[12] = {
		  L"img/gamePlay/redspawn/1.png",
		  L"img/gamePlay/redspawn/2.png",
		  L"img/gamePlay/redspawn/3.png",
		  L"img/gamePlay/redspawn/4.png",
		  L"img/gamePlay/redspawn/5.png",
		  L"img/gamePlay/redspawn/6.png",
		  L"img/gamePlay/redspawn/7.png",
		  L"img/gamePlay/redspawn/8.png",
		  L"img/gamePlay/redspawn/9.png",
		  L"img/gamePlay/redspawn/10.png",
		  L"img/gamePlay/redspawn/11.png",
		  L"img/gamePlay/redspawn/12.png",
	};

	int speed = 3;

	Block() {};
	Block(COLORREF color, COLORREF borderColor, RECT rect) :rect(rect), status(true), color(color), borderColor(borderColor) {};

	Block(COLORREF color, COLORREF borderColor, RECT rect, bool status) : rect(rect), status(status), color(color), borderColor(borderColor) {};

	void print_red_Block(HDC& mDC, Block& redBlock);

	void print_red_res(HDC& mDC, Block& redBlock, int r_n, int cellSize);

	void setRect(RECT rect) {
		this->rect = rect;
	}
};