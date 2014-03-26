#include "ResourceCache.h"

Tileset* ResourceCache::tilesets[24];
PaletteTexture* ResourceCache::entity_textures[73];
PaletteTexture* ResourceCache::flower_texture;
sf::Color ResourceCache::overworld_palettes[768];
DataBlock* ResourceCache::map_palette_indexes;

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
	LoadPalettes();

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

void ResourceCache::LoadPalettes()
{
#ifdef _DEBUG
	cout << "--Loading palettes...";
#endif
	map_palette_indexes = ReadFile(ResourceCache::GetResourceLocation(string("pal\\map_index.dat")).c_str());
	DataBlock* data = ReadFile(ResourceCache::GetResourceLocation(string("pal\\main.dat")).c_str());
	for (int i = 0; i < 768; i++)
	{
		unsigned char r = *data->data++;
		unsigned char g = *data->data++;
		unsigned char b = *data->data++;
		overworld_palettes[i] = sf::Color(r * 8, g * 8, b * 8, 255);
	}
#ifdef _DEBUG
	cout << "Done\n";
#endif
}