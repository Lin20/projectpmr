#pragma once

#include "Common.h"
#include "Constants.h"
#include "Scene.h"
#include "Textbox.h"
#include "MenuCache.h"
#include "ItemStorage.h"
#include "AudioConstants.h"
#include "Pokemon.h"

class BattleScene : public Scene
{
public:
	BattleScene();
	~BattleScene();

private:
	bool wild_battle;
	Pokemon opponent[6];
};
