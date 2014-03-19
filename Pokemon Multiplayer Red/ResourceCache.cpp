#include "ResourceCache.h"

ResourceCache::ResourceCache()
{
}

ResourceCache::~ResourceCache()
{
}

void ResourceCache::LoadAll()
{
	LoadTilesets();
}

void ResourceCache::LoadTilesets()
{
	for (int i = 0; i < 24; i++)
		tilesets[i].loadFromFile(GetResourceLocation(string("tilesets\\").append(to_string(i).append(".png"))));
}