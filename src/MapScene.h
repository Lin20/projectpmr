#pragma once

#include "Common.h"
#include "Constants.h"
#include "Scene.h"
#include "Map.h"
#include "OverworldEntity.h"
#include "NPC.h"
#include "PaletteTexture.h"
#include "Textbox.h"
#include "MenuCache.h"
#include "ItemStorage.h"
#include "AudioConstants.h"

class MapScene : public Scene
{
public:
	MapScene();
	~MapScene();

	void Update() override;
	void Render(sf::RenderWindow*) override;
	void NotifySwitchedTo() override;

	void SwitchMap(unsigned char index);
	void SetPlayerPosition(unsigned char x, unsigned int y);
	void Focus(signed char x, signed char y);
	void FocusFree(int x, int y);

	void DrawMap(sf::RenderWindow* window, Map& map, int connection_index, MapConnection* connection);
	void ClearEntities(bool focused = false);
	void SetPalette(sf::Color* palette, bool only_bg = false);

	bool Interact();
	void WarpTo(Warp& w);
	void Walk();
	void UseEscapeRope();

	Map* GetMap() { return active_map; }
	vector<OverworldEntity*>& GetEntities() { return entities; }
	unsigned char GetLastHealedMap(){ return last_healed_map; }
	bool GetFlag(unsigned int index) { if (index < 4096) return flags[index]; return 0; }
	void SetFlag(unsigned int index, bool b) { if (index < 4096) flags[index] = b; }

	void SetRepel(unsigned char to) { repel_steps = to; }

private:
	Map* active_map;
	sf::View viewport; //this is declared here because the maps are only places where the camera scrolls
	bool flags[16 * 256]; //16 flags per map

	vector<OverworldEntity*> entities;
	OverworldEntity* focus_entity;

	bool can_warp;
	unsigned char previous_palette;
	unsigned char previous_map; //previous overworld map
	unsigned char elevator_map; //where to go when an elevator is exited
	unsigned char last_healed_map; //where to go if the player faints or teleports

	unsigned char repel_steps; //steps remaining for repel
	unsigned char poison_steps;
	unsigned char wild_steps;
	unsigned char wild_transition;
	unsigned char transition_step;
	unsigned char transition_index;
	unsigned char transition_timer;

	unsigned char teleport_stage;
	unsigned char teleport_steps;
	unsigned char teleport_timer;

	Script* active_script;

	void CheckWarp();
	void TryResetWarp();
	void ProcessTeleport();
	void ProcessWildEncounter();
	void ProcessWildTransition();
	void ProcessBattleTransition();
	void DrawBattleTransition(sf::RenderWindow* window);
	void CheckTrainers();
};