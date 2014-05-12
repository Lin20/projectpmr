#include "Engine.h"

Scene* Engine::active_scene = 0;
MapScene* Engine::map_scene = 0;

unsigned char Engine::game_state = 0;

void Engine::Initialize()
{
	ResourceCache::LoadAll();
	Players::Initialize();

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