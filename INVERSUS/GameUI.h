#pragma once

#include<vector>

#include<atlimage.h>
#include"GameStateManager.h"
#include"Block.h"

class GameUI {
private:
	CImage healthBar;
	int level;
	int player;


	HDC* mDC;
	GameStateManager* gameStateManager;

	int exp; //°æÇèÄ¡
	int hp;
	int score;

public:

	int countDown = 0;
	bool countDownStatus = false;
	int line_size = 0;
	int cellSize = 0;

	RECT gameBordRect;

	GameUI(GameStateManager* instance) :gameStateManager(instance), hp(300), exp(0), score(0) {};
	
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
	void drawHP(HDC& mDC, const RECT& rect, int hp, GameUI& gameUi);
	void drawExp(HDC& mDC, const RECT& rect, int exp);
	void mainAsset(HDC& mDC, const Block& mainBlock);
	void setBlackBlock(std::vector<Block>& blocks, int cellSize);
	void printBlackBlock(std::vector<Block> blocks, HDC& mDC);

};