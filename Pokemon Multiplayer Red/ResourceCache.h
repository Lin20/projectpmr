#pragma once

#include <string>
#include <SFML\Graphics.hpp>

#include "Common.h"
#include "Tileset.h"
#include "PaletteTexture.h"

#ifdef _DEBUG
#include <iostream>
#endif

using namespace std;

#define RESOURCE_DIR "C:\\red dumps\\"

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

private:
	//tilesets
	static Tileset* tilesets[24];
	static PaletteTexture* entity_textures[73];
	static PaletteTexture* flower_texture;

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
};

