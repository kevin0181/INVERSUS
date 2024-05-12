#pragma once

#include<atlimage.h>
#include <string>

#include "GameState.h"

class GameStateManager {
private:
	GameState currentState;
	CImage cImage;

	int player;
	int level;

public:
	void setCurrentState(GameState state);
	void setImage(LPCTSTR path);
	void DrawImage(HDC& mDC, RECT rect);

	CImage getImage() {
		return cImage;
	}

	void setPlayer(int player) {
		this->player = player;
	}

	void setLevel(int level) {
		this->level = level;
	}

	int getLevel() {
		return level;
	}

	int getPlayer() {
		return player;
	}

	GameState getState();
};