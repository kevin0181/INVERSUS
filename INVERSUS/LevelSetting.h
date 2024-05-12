#pragma once

#include "GameStateManager.h"

class LevelSetting {
private:
	int level;

	GameStateManager* gameStateManager;
public:
	LevelSetting(GameStateManager* instance) :gameStateManager(instance) {};
	void level_setting(WPARAM wParam);
};