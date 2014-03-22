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

	void LoadAll();
	void LoadTilesets();
	void LoadEntities();
	inline static string GetResourceLocation(string name) { return name.insert(0, RESOURCE_DIR); }

	inline Tileset* GetTileset(unsigned char index) 
	{
		if (index >= 24)
			return 0;
		return tilesets[index];
	}

	inline sf::Texture* GetEntityTexture(unsigned char index)
	{
		if (index >= 73)
			return 0;
		return entity_textures[index];
	}

private:
	Tileset* tilesets[24];
	sf::Texture* entity_textures[73];
};

