#pragma once

#include "Common.h"
#include "Constants.h"
#include "Scene.h"
#include "MapScene.h"
#include "Players.h"
#include "MusicPlayer.h"

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

	static MusicPlayer& GetMusicPlayer() { return music_player; }

private:
	static Scene* active_scene;
	static MapScene* map_scene; //the main scene where the player walks around

	static unsigned char game_state;

	static MusicPlayer music_player;
	static void InitializeAudio();
};