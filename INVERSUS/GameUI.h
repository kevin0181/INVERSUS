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

	int exp; //°æÇèÄ¡
	int hp;
	int score;

public:

	int countDown = 0;
	bool countDownStatus = false;

	GameUI(GameStateManager* instance) :gameStateManager(instance),hp(300),exp(0),score(0) {};
	
	void setMDC(HDC* mDC) {
		this->mDC = mDC;
	}

	void setGameBord(RECT rect) {
		gameBordRect = rect;
	}

	RECT getGameBord() {
		return gameBordRect;
	}

	int getHp() {
		return hp;
	}

	void drawGameUI(HDC& mDC, GameUI& gameUi, RECT rect);
};