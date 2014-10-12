#pragma once

#include "Common.h"
#include "Constants.h"
#include "Scene.h"
#include "MapScene.h"
#include "BattleScene.h"
#include "Players.h"
#include "SFPlayer.h"

class Engine
{
public:
	static void Initialize();
	static void Update();
	static void Render(sf::RenderWindow* w);

	static void SwitchState(unsigned char state);

	static void Release();

	static Scene* GetActiveScene() { return active_scene; }
	static unsigned char GetState() { return game_state; }
	static MapScene* GetMapScene() { return map_scene; }
	static BattleScene* GetBattleScene() { return battle_scene; }

	static SFPlayer& GetMusicPlayer() { return music_player; }
	static SFPlayer& GetWorldSounds() { return world_sounds; }
	static SFPlayer& GetCryPlayer() { return cry_player; }

private:
	static Scene* active_scene;
	static MapScene* map_scene; //the main scene where the player walks around
	static BattleScene* battle_scene;

	static unsigned char game_state;

	static SFPlayer music_player;
	static SFPlayer world_sounds;
	static SFPlayer cry_player;
	static void InitializeAudio();
};