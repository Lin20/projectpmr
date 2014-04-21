#include "ResourceCache.h"

Tileset* ResourceCache::tilesets[24];
PaletteTexture* ResourceCache::entity_textures[73];
PaletteTexture* ResourceCache::flower_texture = 0;

sf::Color ResourceCache::overworld_palettes[768];
DataBlock* ResourceCache::map_palette_indexes = 0;

DataBlock* ResourceCache::ledges = 0;
DataBlock* ResourceCache::jump_coordinates = 0;
PaletteTexture* ResourceCache::shadow_texture = 0;

PaletteTexture* ResourceCache::menu_texture = 0;
PaletteTexture* ResourceCache::font_texture = 0;
DataBlock* ResourceCache::ascii_table = 0;
string ResourceCache::item_names[256];
bool ResourceCache::key_items[256];

ResourceCache::ResourceCache()
{
}

ResourceCache::~ResourceCache()
{

}

void ResourceCache::ReleaseResources()
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
	if (map_palette_indexes)
		delete map_palette_indexes;
	if (ledges)
		delete ledges;
	if (jump_coordinates)
		delete jump_coordinates;
	if (shadow_texture)
		delete shadow_texture;

	if (menu_texture)
		delete menu_texture;
	if (font_texture)
		delete font_texture;
	if (ascii_table)
		delete ascii_table;
}

void ResourceCache::LoadAll()
{
#ifdef _DEBUG
	cout << "Loading resources...\n";
#endif

	LoadTilesets();
	LoadEntities();
	LoadPalettes();
	LoadMisc();

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
	flower_texture->loadFromFile(ResourceCache::GetResourceLocation(string("tilesets/flower.png")));
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
		entity_textures[i]->loadFromFile(ResourceCache::GetResourceLocation(string("npcs/").append(itos(i)).append(".png")));
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
	map_palette_indexes = ReadFile(ResourceCache::GetResourceLocation(string("pal/map_index.dat")).c_str());
	DataBlock* data = ReadFile(ResourceCache::GetResourceLocation(string("pal/main.dat")).c_str());
	for (int i = 0; i < 768; i++)
	{
		unsigned char r = *data->data++;
		unsigned char g = *data->data++;
		unsigned char b = *data->data++;
		overworld_palettes[i] = sf::Color(r * 8, g * 8, b * 8, 255);
	}
	delete data;
#ifdef _DEBUG
	cout << "Done\n";
#endif
}

void ResourceCache::LoadMisc()
{
#ifdef _DEBUG
	cout << "--Loading misc...";
#endif
	ledges = ReadFile(ResourceCache::GetResourceLocation(string("misc/ledges.dat")).c_str());
	jump_coordinates = ReadFile(ResourceCache::GetResourceLocation(string("misc/jumps.dat")).c_str());

	shadow_texture = new PaletteTexture();
	shadow_texture->loadFromFile(ResourceCache::GetResourceLocation(string("npcs/shadow.png")));

	menu_texture = new PaletteTexture();
	menu_texture->loadFromFile(ResourceCache::GetResourceLocation(string("misc/menu.png")));
	font_texture = new PaletteTexture();
	font_texture->loadFromFile(ResourceCache::GetResourceLocation(string("misc/font.png")));
	ascii_table = ReadFile(ResourceCache::GetResourceLocation(string("misc/ascii_table.dat")).c_str());

	DataBlock* d = ReadFile(ResourceCache::GetResourceLocation(string("misc/items.dat")).c_str());
	unsigned char* p = d->data;
	for (int i = 1; i < 256; i++)
	{
		string s;
		while ((unsigned int)(p - d->data_start) < d->size && *p != MESSAGE_ENDNAME)
			s.insert(s.begin() + s.length(), (char)*p++);
		p++;
		item_names[i] = s;
	}

	delete d;
	d = ReadFile(ResourceCache::GetResourceLocation(string("misc/keyitems.dat")).c_str());
	memcpy(key_items, d->data, 256);
	delete d;

#ifdef _DEBUG
	cout << "Done\n";
#endif
}