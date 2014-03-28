#pragma once

#include <SFML/Graphics.hpp>

#include "TileMap.h"
#include "ResourceCache.h"
#include "Map.h"

class OverworldEntity : public TileMap
{
public:
	OverworldEntity(Map* on_map, unsigned char index, unsigned char x, unsigned char y, unsigned char direction, bool npc = true);
	~OverworldEntity();

	void Update();
	void Render(sf::RenderWindow* window);
	void Face(unsigned char direction);
	void StartMoving(unsigned char direction);
	void StopMoving();
	void Move(unsigned char direction, unsigned char steps = 1);
	
	inline bool Snapped() { return x % 16 == 0 && y % 16 == 0; }

	int x;
	int y;
private:
	unsigned char index;
	Map* on_map;
	bool is_npc;

	//movement stuff
	unsigned char direction;
	unsigned char steps_remaining; //used for npcs for walking for x tiles
	float step_timer;
	float animation_timer;
	float turn_delay_timer;
	bool forced_steps;
	unsigned char step_frame;
	unsigned char movement_direction; //what direction are we making the entity move in?
	unsigned char movement_type; //used for normal walking or hopping down a ledge

	unsigned char jump_index; //the index of the jump sequence stored in ResourceCache::jump_coordinates
	int jump_x;
	int jump_y;

	sf::Sprite shadow8x8;
};