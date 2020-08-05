#pragma once

#include <string>
#include <SFML/Graphics.hpp>

#include "Common.h"
#include "Tileset.h"
#include "PaletteTexture.h"
#include "Utils.h"
#include "Events.h"

#ifdef _DEBUG
#include <iostream>
#endif

#ifdef _WIN32
#define RESOURCE_DIR "resources/"
#else
#define RESOURCE_DIR "resources/"
#endif


using namespace std;

class ResourceCache
{
public:
	ResourceCache();
	~ResourceCache();

	static void LoadAll();
	static void LoadTilesets();
	static void LoadEntities();
	static void LoadPalettes();
	static void LoadMisc();
	static void LoadItems();
	static void LoadPokemon();
	static void LoadMoves();
	static void LoadBattleData();
	static void LoadTrainers();

	inline static string GetResourceLocation(string name) { return name.insert(0, RESOURCE_DIR); }
	static void ReleaseResources();

	inline static Tileset* GetTileset(unsigned char index)
	{
		if (index >= 24)
			return 0;
		return tilesets[index];
	}

	inline static PaletteTexture* GetEntityTexture(unsigned char index)
	{
		if (index >= 73)
			return 0;
		return entity_textures[index];
	}

	inline static PaletteTexture* GetEmotionBubbles()
	{
		return emotion_bubbles;
	}

	inline static PaletteTexture* GetFlowerTexture() { return flower_texture; }

	static sf::Color* GetPalette(unsigned char index)
	{
		return &overworld_palettes[index * 4];
	}

	inline static unsigned char GetMapPaletteIndex(unsigned char index)
	{
		if (map_palette_indexes)
			return map_palette_indexes->data[index];
		return 0xFF;
	}

	inline static DataBlock* GetLedges() { return ledges; }

	inline static DataBlock* GetJumpCoordinates() { return jump_coordinates; }

	inline static PaletteTexture* GetShadowTexture() { return shadow_texture; }

	inline static PaletteTexture* GetMenuTexture() { return menu_texture; }
	inline static PaletteTexture* GetFontTexture() { return font_texture; }
	inline static DataBlock* GetAsciiTable() { return ascii_table; }
	inline static string& GetItemName(unsigned char index) { return item_names[index]; }
	inline static bool IsKeyItem(unsigned char index) { return key_items[index]; }
	inline static unsigned char GetItemUse(unsigned char index) { return item_uses[index]; }

	inline static DataBlock* GetPokemonStats(unsigned char index) { return pokemon_stats[index]; }
	inline static unsigned char GetPokedexIndex(unsigned char created_index) { if (pokemon_indexes) return pokemon_indexes->data[created_index]; return 0; }
	inline static string& GetPokemonName(unsigned char created_index) { return pokemon_names[created_index]; }
	inline static PaletteTexture* GetStatusesTexture(unsigned char color) { return statuses_texture[color % 4]; }
	inline static PaletteTexture* GetPokemonIcons() { return pokemon_icons; }
	inline static unsigned char GetIconIndex(unsigned char pokedex_index) { if (icon_indexes) return icon_indexes->data[pokedex_index]; return 0; }
	inline static unsigned char GetPokemonPaletteIndex(unsigned char index) { if (mon_palette_indexes) return mon_palette_indexes->data[index]; return 0; }
	inline static PaletteTexture* GetPokemonFront(unsigned char index) { return pokemon_front[index]; }
	inline static PaletteTexture* GetPokemonBack(unsigned char index) { return pokemon_back[index]; }
	inline static DataBlock* GetPokemonLeveling(unsigned char index) { return pokemon_leveling[index]; }

	inline static string& GetMoveName(unsigned char index) { return move_names[index]; }
	inline static DataBlock* GetMoveData() { return move_data; }

	inline static FlyPoint& GetFlyPoint(unsigned char index) { if (index > 12) index = 0; return fly_points[index]; }
	inline static bool CanUseEscapeRope(unsigned char tileset) { for (unsigned int i = 0; i < escape_rope_tilesets->size; i++) if (escape_rope_tilesets->data[i] == tileset) return true; return false; }
	inline static bool CanUseBicycle(unsigned char tileset) { for (unsigned int i = 0; i < bicycle_tilesets->size; i++) if (bicycle_tilesets->data[i] == tileset) return true; return false; }

	inline static unsigned char GetMusicIndex(unsigned char map) { return music_indexes[map]; }

	inline static unsigned char* GetWildChances() { return wild_chances; }
	inline static Transition& GetBattleTransition(unsigned char index) { return transitions[index]; }
	inline static unsigned char GetTrainerMusic(unsigned char index) { return trainer_music[index]; }

	inline static PaletteTexture* GetTrainerFront(unsigned char index) { return trainer_front[index]; }
	inline static PaletteTexture* GetRedBack() { return red_back; }
	inline static PaletteTexture* GetManBack() { return man_back; }
	inline static string& GetTrainerName(unsigned char index) { return trainer_names[index]; }

private:
	//tilesets
	static Tileset* tilesets[24];
	static PaletteTexture* entity_textures[73];
	static PaletteTexture* flower_texture;
	static PaletteTexture* emotion_bubbles;

	//map palettes
	static sf::Color overworld_palettes[768];
	static DataBlock* map_palette_indexes;

	//ledge stuff
	static DataBlock* ledges;
	static DataBlock* jump_coordinates;
	static PaletteTexture* shadow_texture;

	//textbox
	static PaletteTexture* menu_texture;
	static PaletteTexture* font_texture;
	static DataBlock* ascii_table;

	//items
	static string item_names[256];
	static bool key_items[256];
	static unsigned char item_uses[256];

	//pokemon stuff
	static DataBlock* pokemon_stats[256];
	static DataBlock* pokemon_indexes;
	static string pokemon_names[256];
	static PaletteTexture* statuses_texture[4]; //3 for each hp bar color, 1 for the pokeballs
	static PaletteTexture* pokemon_icons;
	static DataBlock* icon_indexes;
	static PaletteTexture* pokemon_front[256];
	static PaletteTexture* pokemon_back[256];
	static DataBlock* mon_palette_indexes;
	static DataBlock* pokemon_leveling[256];

	//move stuff
	static string move_names[256];
	static DataBlock* move_data;

	//misc
	static FlyPoint fly_points[13];
	static DataBlock* escape_rope_tilesets;
	static DataBlock* bicycle_tilesets;
	static unsigned char music_indexes[256];
	static unsigned char trainer_music[256];
	
	//battle
	static unsigned char wild_chances[10];
	static Transition transitions[8];

	//trainer
	static PaletteTexture* trainer_front[256];
	static PaletteTexture* red_back;
	static PaletteTexture* man_back;
	static string trainer_names[256];
};
