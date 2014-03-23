#include "ResourceCache.h"

Tileset* ResourceCache::tilesets[24];
PaletteTexture* ResourceCache::entity_textures[73];
PaletteTexture* ResourceCache::flower_texture;

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

	for (int i = 0; i < 73; i++)
	{
		if (entity_textures[i])
			delete entity_textures[i];
	}
	if (flower_texture)
		delete flower_texture;
}

void ResourceCache::LoadAll()
{
#ifdef _DEBUG
	cout << "Loading resources...\n";
#endif

	LoadTilesets();
	LoadEntities();

#ifdef _DEBUG
	cout << "Done\n";
#endif
}

void ResourceCache::LoadTilesets()
{
#ifdef _DEBUG
	cout << "--Loading tilesets...";
#endif
	flower_texture = new PaletteTexture();
	flower_texture->loadFromFile(ResourceCache::GetResourceLocation(string("tilesets\\flower.png")));
	for (int i = 0; i < 24; i++)
	{
		tilesets[i] = new Tileset(i);
	}
#ifdef _DEBUG
	cout << "Done\n";
#endif
}

void ResourceCache::LoadEntities()
{
#ifdef _DEBUG
	cout << "--Loading entities...";
#endif
	for (int i = 0; i < 73; i++)
	{
		entity_textures[i] = new PaletteTexture();
		entity_textures[i]->loadFromFile(ResourceCache::GetResourceLocation(string("npcs\\").append(to_string(i)).append(".png")));
	}
#ifdef _DEBUG
	cout << "Done\n";
#endif
}