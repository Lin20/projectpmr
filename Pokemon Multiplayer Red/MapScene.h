#pragma once

#include "Scene.h"
#include "Map.h"

class MapScene : public Scene
{
public:
	MapScene(ResourceCache* r);
	~MapScene();

	void Update() override;
	void Render(sf::RenderWindow*) override;

private:
	Map active_map;
};

