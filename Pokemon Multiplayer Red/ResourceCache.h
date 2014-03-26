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
	inline static string GetResourceLocation(string name) { return name.insert(0, RESOURCE_DIR); }

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

private:
	static Tileset* tilesets[24];
	static PaletteTexture* entity_textures[73];
	static PaletteTexture* flower_texture;
	static sf::Color overworld_palettes[768];
	static DataBlock* map_palette_indexes;
};

