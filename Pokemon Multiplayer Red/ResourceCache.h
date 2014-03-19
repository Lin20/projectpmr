#pragma once

#include <string>
#include <SFML\Graphics.hpp>

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
	sf::Texture tilesets[24];
};

