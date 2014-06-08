#include "Engine.h"

Scene* Engine::active_scene = 0;
MapScene* Engine::map_scene = 0;
MusicPlayer Engine::music_player;

unsigned char Engine::game_state = 0;

void Engine::Initialize()
{
	
	ResourceCache::LoadAll();
	Players::Initialize();
	InitializeAudio();

	//Initialize the scenes
	map_scene = new MapScene();

	SwitchState(States::OVERWORLD);

}

void Engine::Update()
{
	active_scene->Update();
}

void Engine::Render(sf::RenderWindow* window)
{
	active_scene->Render(window);
}

void Engine::SwitchState(unsigned char s)
{
	game_state = s;
	switch (game_state)
	{
	case States::OVERWORLD:
		active_scene = (Scene*)map_scene;
		break;
	}

	active_scene->NotifySwitchedTo();
}

void Engine::Release()
{
	if (map_scene)
	{
		delete map_scene;
		map_scene = 0;
	}
}

void Engine::InitializeAudio()
{
	const char* err = music_player.Initialize();
	if (err)
	{
#ifdef _DEBUG
		std::cout << "Failed to start music player. " << err << "\n";
#endif
	}

	err = music_player.LoadFile(ResourceCache::GetResourceLocation(string("audio/music.gbs")).c_str());
	if (err)
	{
#ifdef _DEBUG
		std::cout << "Failed to load music. " << err << "\n";
#endif
		return;
	}

	music_player.Play(0);
}
