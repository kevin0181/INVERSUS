#pragma once
#include "GameStateManager.h"

class PlayerSetting {
private:
	int player;
	GameStateManager* gameStateManager;
public:

	PlayerSetting(GameStateManager* instance) : gameStateManager(instance) {};

	void game_setting(WPARAM wParam);
};