#pragma once
#include "GameStateManager.h"

#include <map>

class Setting {
	
	int se_n;
	int music_n;

	bool invincibility; //무적 모드

	GameStateManager* gameStateManager;
public:
	Setting(GameStateManager* instance) :gameStateManager(instance), se_n(1), music_n(0), invincibility(false) {};

	void setting(WPARAM wParam, HWND& hWnd);
	void draw_setting(HDC& mDC, RECT rect);

	int getMusic_n() {
		return music_n;
	}

	bool getInvincibility() const {
		return invincibility;
	}

	
};