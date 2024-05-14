#pragma once

#include <vector>

#include "GameStateManager.h"
#include "GameUI.h"

class LevelSetting {
private:
	int level;

	GameStateManager* gameStateManager;
	GameUI* gameUi;
public:
	LevelSetting(GameStateManager* instance, GameUI* g_instance) :gameStateManager(instance), gameUi(g_instance), level(0) {};
	void level_setting(WPARAM wParam, HWND& hWnd, RECT rect);
	void gameBoard(RECT rect);
	void setEnter(HWND hWnd, RECT rect);
};