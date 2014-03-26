#pragma once

#include "Constants.h"
#include "Scene.h"
#include "Map.h"
#include "OverworldEntity.h"
#include "PaletteTexture.h"

class MapScene : public Scene
{
public:
	MapScene();
	~MapScene();

	void Update() override;
	void Render(sf::RenderWindow*) override;

	void SwitchMap(unsigned char index);
	void SetPlayerPosition(unsigned char x, unsigned int y);
	void Focus(signed char x, signed char y);
	void FocusFree(int x, int y);

	void DrawMap(sf::RenderWindow* window, Map& map, int connection_index, MapConnection* connection);

private:
	Map* active_map;
	sf::View viewport; //this is declared here because the maps are only places where the camera scrolls

	OverworldEntity* test_entity;

	bool key_down;
};

