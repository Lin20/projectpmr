#include "ResourceCache.h"

ResourceCache::ResourceCache()
{
}

ResourceCache::~ResourceCache()
{
	for (int i = 0; i < 24; i++)
	{
		if (tilesets[i])
			delete tilesets[i];
	}
}

void ResourceCache::LoadAll()
{
#ifdef _DEBUG
	cout << "Loading resources...\n";
#endif

	LoadTilesets();

#ifdef _DEBUG
	cout << "Done\n";
#endif
}

void ResourceCache::LoadTilesets()
{
#ifdef _DEBUG
	cout << "--Loading tilesets...";
#endif
	water_tile.loadFromFile(ResourceCache::GetResourceLocation(string("tilesets\\animation\\water.png")));

	for (int i = 0; i < 24; i++)
	{
		tilesets[i] = new Tileset(i, &water_tile);
	}
#ifdef _DEBUG
	cout << "Done\n";
#endif
}