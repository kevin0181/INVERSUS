#pragma once
#include<atlimage.h>
#include"GameStateManager.h"

class GameUI {
private:
	CImage healthBar;
	int level;
	int player;

	RECT gameBordRect;

	HDC* mDC;
	GameStateManager* gameStateManager;

	
public:

	int countDown = 0;
	bool countDownStatus = false;

	GameUI(GameStateManager* instance) :gameStateManager(instance) {};
	
	void setMDC(HDC* mDC) {
		this->mDC = mDC;
	}

	void setGameBord(RECT rect) {
		gameBordRect = rect;
	}

	RECT getGameBord() {
		return gameBordRect;
	}

	void drawGameUI(HDC& mDC, GameUI& gameUi);
};