#pragma once

#include <SFML/Graphics.hpp>

#include "TileMap.h"
#include "ResourceCache.h"
#include "Map.h"

class OverworldEntity : public TileMap
{
public:
	OverworldEntity(Map* on_map, unsigned char index, unsigned char x, unsigned char y, unsigned char direction);
	~OverworldEntity();

	void Update();
	void Render(sf::RenderWindow* window);
	void Face(unsigned char direction);
	void Move(unsigned char direction, unsigned char steps = 1);
	
	inline bool Snapped() { return x % 16 == 0 && y % 16 == 0; }

	int x;
	int y;
private:
	unsigned char index;
	Map* on_map;

	//movement stuff
	unsigned char direction;
	unsigned char steps_remaining;
	float step_timer;
	unsigned char step_frame;
};