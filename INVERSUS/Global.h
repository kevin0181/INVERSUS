#include "GameStateManager.h"
#include "LevelSetting.h"
#include "GameUI.h"
#include "PlayerSetting.h"
#include "Setting.h"

static GameStateManager gameStateManager;
static PlayerSetting playerSetting(&gameStateManager);
static LevelSetting levelSetting(&gameStateManager);
static GameUI gameUi(&gameStateManager);
static Setting setting(&gameStateManager);