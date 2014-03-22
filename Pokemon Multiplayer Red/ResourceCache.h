#pragma once

#include <string>
#include <SFML\Graphics.hpp>

#include "Common.h"
#include "Tileset.h"

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
	inline static string GetResourceLocation(string name) { return name.insert(0, RESOURCE_DIR); }

	inline static Tileset* GetTileset(unsigned char index) 
	{
		if (index >= 24)
			return 0;
		return tilesets[index];
	}

	inline static sf::Texture* GetEntityTexture(unsigned char index)
	{
		if (index >= 73)
			return 0;
		return entity_textures[index];
	}

	inline static sf::Texture* GetFlowerTexture() { return flower_texture; }

private:
	static Tileset* tilesets[24];
	static sf::Texture* entity_textures[73];
	static sf::Texture* flower_texture;
};

