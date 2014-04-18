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
	void ForceStop();
	void Move(unsigned char direction, unsigned char steps = 1);
	
	inline bool Snapped() { return x % 16 == 0 && y % 16 == 0; }
	inline bool Moving() { return step_timer > 0; }
	inline unsigned char GetDirection() { return direction; }
	inline unsigned char GetMovementDirection() { return movement_direction; }

	void SetMap(Map* m)
	{
		this->on_map = m;
	}

	void SetPalette(sf::Color* pal)
	{
		sf::Color palette[4] = { sf::Color(0, 0, 0, 0), *pal, pal[1], pal[3] };
		tiles_tex->SetPalette(palette);
		if (!is_npc)
			ResourceCache::GetShadowTexture()->SetPalette(palette);
	}

	int x;
	int y;
private:
	unsigned char index;
	Map* on_map;
	bool is_npc;
	bool can_move;

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