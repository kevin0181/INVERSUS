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
	GameUI(GameStateManager* instance, HDC* m) :gameStateManager(instance), mDC(m) {};
	
	void setGameBord(RECT rect) {
		gameBordRect = rect;
	}

	RECT getGameBord() {
		return gameBordRect;
	}

	void drawGameUI();
};