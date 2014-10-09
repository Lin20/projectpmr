#pragma once

#include <SFML/Graphics.hpp>
#include <functional>

#include "TileMap.h"
#include "ResourceCache.h"
#include "Map.h"
#include "Script.h"

class OverworldEntity : public TileMap
{
public:
	OverworldEntity(Map* on_map, unsigned char index, unsigned char sprite, unsigned char x, unsigned char y, unsigned char direction, bool npc = true, Script* _script = 0, std::function<void()> step_callback = nullptr);
	virtual ~OverworldEntity();

	virtual void Update();
	virtual void Render(sf::RenderWindow* window, int offset_x = 0, int offset_y = 0);
	void Face(unsigned char direction);
	void StartMoving(unsigned char direction);
	void StopMoving();
	void ForceStop();
	void Move(unsigned char direction, unsigned char steps = 1, bool fast = false);
	void SetSprite(unsigned char index);
	void ExecuteScript(Script* script);
	
	inline bool Snapped() { return x % 16 == 0 && y % 16 == 0; }
	inline bool Moving() { return step_timer > 0; }
	inline unsigned char GetStepsRemaining() { return steps_remaining; }
	inline unsigned char GetDirection() { return direction; }
	inline unsigned char GetMovementDirection() { return movement_direction; }
	inline Script* GetScript() { return script; }
	inline void SetScriptState(bool enabled) { script_enabled = enabled; }
	inline void SetEntityGhosting(bool b) { allow_entity_ghosting = b; }
	inline void SetEmote(unsigned char e) { emotion_bubble = e; }
	inline bool Frozen() { return frozen; }
	inline void SetFrozen(bool b) { frozen = b; }
	inline unsigned char GetIndex() { return sprite; }

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
	int offset_y;
	int index;
protected:
	unsigned char sprite;
	Map* on_map;
	bool is_npc;
	bool frozen;

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
	bool allow_entity_ghosting;
	bool force_fast;

	unsigned char jump_index; //the index of the jump sequence stored in ResourceCache::jump_coordinates
	int jump_x;
	int jump_y;
	
	Script* script;
	Script* temp_script;
	bool script_enabled;
	unsigned char emotion_bubble;
	TileMap* emotion_texture;

	sf::Sprite shadow8x8;

	std::function<void()> step_callback;
};