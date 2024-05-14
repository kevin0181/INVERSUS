#include <string>
#include "GameStateManager.h"
#include "GameState.h"

void GameStateManager::setCurrentState(GameState gameState) {
	currentState = gameState;
}

void GameStateManager::setImage(LPCTSTR path) {
	cImage.Destroy();
	cImage.Load(path);
}

void GameStateManager::destroyImg() {
	cImage.Destroy();
}

void GameStateManager::DrawImage(HDC& mDC, RECT rect) {

	if (currentState == GameState::GAMEPLAY) {
		cImage.Draw(mDC, 0, 0, cImage.GetWidth(), cImage.GetHeight(), 0, 0, cImage.GetWidth(), cImage.GetHeight()); //이미지 전체 화면
		return;
	}

	cImage.Draw(mDC, 0, 0, rect.right, rect.bottom, 0, 0, cImage.GetWidth(), cImage.GetHeight()); //이미지 전체 화면
}

GameState GameStateManager::getState() {
	return currentState;
}