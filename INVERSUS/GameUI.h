#pragma once
#include<atlimage.h>
#include"GameStateManager.h"

class GameUI {
private:
	CImage healthBar;
	int level;
	int player;

	RECT gameBordRect;

	GameStateManager* gameStateManager;
public:
	GameUI(GameStateManager* instance) :gameStateManager(instance) {};
	
	void setGameBord(RECT rect) {
		gameBordRect = rect;
	}

	RECT getGameBord() {
		return gameBordRect;
	}
};