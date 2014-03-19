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
	LoadTilesets();
}

void ResourceCache::LoadTilesets()
{
	for (int i = 0; i < 24; i++)
		tilesets[i] = new Tileset(i);
}