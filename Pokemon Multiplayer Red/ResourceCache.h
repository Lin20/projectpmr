#pragma once

#include <string>
#include <SFML\Graphics.hpp>

#include "Common.h"
#include "Tileset.h"

using namespace std;

#define RESOURCE_DIR "C:\\red dumps\\"

class ResourceCache
{
public:
	ResourceCache();
	~ResourceCache();

	void LoadAll();
	void LoadTilesets();
	inline static string GetResourceLocation(string name) { return name.insert(0, RESOURCE_DIR); }

private:
	Tileset* tilesets[24];
};

