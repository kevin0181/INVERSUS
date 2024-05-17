#pragma once

#include<vector>

#include "GameStateManager.h"
#include "LevelSetting.h"
#include "GameUI.h"
#include "PlayerSetting.h"
#include "Setting.h"

static GameStateManager gameStateManager;
static PlayerSetting playerSetting(&gameStateManager);
static GameUI gameUi(&gameStateManager);
static LevelSetting levelSetting(&gameStateManager, &gameUi);
static Setting setting(&gameStateManager);

static int backgroundS = -2500;