#include "ResourceCache.h"

Tileset* ResourceCache::tilesets[24];
PaletteTexture* ResourceCache::entity_textures[73];
PaletteTexture* ResourceCache::flower_texture = 0;
PaletteTexture* ResourceCache::emotion_bubbles = 0;

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
unsigned char ResourceCache::item_uses[256];

DataBlock* ResourceCache::pokemon_stats[256];
DataBlock* ResourceCache::pokemon_indexes = 0;
string ResourceCache::pokemon_names[256];
PaletteTexture* ResourceCache::statuses_texture[3];
PaletteTexture* ResourceCache::pokemon_icons = 0;
DataBlock* ResourceCache::icon_indexes = 0;
PaletteTexture* ResourceCache::pokemon_front[256];
PaletteTexture* ResourceCache::pokemon_back[256];
DataBlock* ResourceCache::mon_palette_indexes = 0;
DataBlock* ResourceCache::pokemon_leveling[256];

string ResourceCache::move_names[256];
DataBlock* ResourceCache::move_data = 0;

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
	if (emotion_bubbles)
		delete emotion_bubbles;
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

	for (int i = 0; i < 256; i++)
	{
		if (pokemon_stats[i])
			delete pokemon_stats[i];
	}
	if (pokemon_indexes)
		delete pokemon_indexes;
	for (int i = 0; i < 3; i++)
	{
		if (statuses_texture[i])
			delete statuses_texture[i];
	}
	if (pokemon_icons)
		delete pokemon_icons;
	if (icon_indexes)
		delete icon_indexes;
	if (mon_palette_indexes)
		delete mon_palette_indexes;
	for (int i = 0; i < 256; i++)
	{
		if (pokemon_front[i])
			delete pokemon_front[i];
		if (pokemon_back[i])
			delete pokemon_back[i];
		if (pokemon_leveling[i])
			delete pokemon_leveling[i];
	}

	if (move_data)
		delete move_data;
}

void ResourceCache::LoadAll()
{
	srand((unsigned int)time(0));

#ifdef _DEBUG
	cout << "Loading resources...\n";
#endif

	LoadTilesets();
	LoadEntities();
	LoadPalettes();
	LoadMisc();
	LoadItems();
	LoadPokemon();
	LoadMoves();

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
	emotion_bubbles = new PaletteTexture();
	emotion_bubbles->loadFromFile(ResourceCache::GetResourceLocation(string("misc/emotionbubbles.png")));
	
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

#ifdef _DEBUG
	cout << "Done\n";
#endif
}

void ResourceCache::LoadItems()
{
#ifdef _DEBUG
	cout << "--Loading items...";
#endif
	DataBlock* d = ReadFile(ResourceCache::GetResourceLocation(string("items/items.dat")).c_str());
	unsigned char* p = d->data;
	for (int i = 1; i < 256; i++)
	{
		string s;
		while ((unsigned int)(p - d->data_start) < d->size && *p != MESSAGE_ENDNAME)
			s.insert(s.begin() + s.length(), (char)*p++);
		p++;
		item_names[i] = s;
		//these item names get reported as a memory leak
		//however since theyre static not manually allocated, they will be deleted when the program terminates
		//the leak reportings are false
	}

	delete d;
	d = ReadFile(ResourceCache::GetResourceLocation(string("items/keyitems.dat")).c_str());
	memcpy(key_items, d->data, 256);
	delete d;

	d = ReadFile(ResourceCache::GetResourceLocation(string("items/uses.dat")).c_str());
	memcpy(item_uses, d->data, 256);
	delete d;

#ifdef _DEBUG
	cout << "Done\n";
#endif
}

void ResourceCache::LoadPokemon()
{
#ifdef _DEBUG
	cout << "--Loading Pokemon...";
#endif
	for (int i = 0; i < 256; i++)
	{
		DataBlock* d = ReadFile(ResourceCache::GetResourceLocation(string("pokemon/stats/").append(itos(i)).append(".dat")));
		if (d)
			pokemon_stats[i] = d;
	}

	pokemon_indexes = ReadFile(ResourceCache::GetResourceLocation(string("pokemon/dex_indexes.dat")).c_str());

	DataBlock* d = ReadFile(ResourceCache::GetResourceLocation(string("pokemon/names.dat")).c_str());
	unsigned char* p = d->data;
	for (int i = 0; i < 256; i++)
	{
		string s;
		while ((unsigned int)(p - d->data_start) < d->size && *p != MESSAGE_ENDNAME)
			s.insert(s.begin() + s.length(), (char)*p++);
		p++;
		pokemon_names[i] = s;
		//these pokemon names get reported as a memory leak
		//however since theyre static not manually allocated, they will be deleted when the program terminates
		//the leak reportings are false
	}
	delete d;

	for (int i = 0; i < 3; i++)
	{
		statuses_texture[i] = new PaletteTexture();
		statuses_texture[i]->loadFromFile(ResourceCache::GetResourceLocation(string("gui/hp_statuses.png")));
	}
	pokemon_icons = new PaletteTexture();
	pokemon_icons->loadFromFile(ResourceCache::GetResourceLocation(string("pokemon/icons.png")));
	icon_indexes = ReadFile(ResourceCache::GetResourceLocation(string("pokemon/icon_indexes.dat")).c_str());

	mon_palette_indexes = ReadFile(ResourceCache::GetResourceLocation(string("pal/mon_index.dat")).c_str());
	for (int i = 0; i < 256; i++)
	{
		pokemon_front[i] = new PaletteTexture();
		pokemon_front[i]->loadFromFile(ResourceCache::GetResourceLocation(string("pokemon/front/").append(itos(i)).append(".png")));
		pokemon_back[i] = new PaletteTexture();
		pokemon_back[i]->loadFromFile(ResourceCache::GetResourceLocation(string("pokemon/back/").append(itos(i)).append(".png")));

		pokemon_front[i]->SetPalette(GetPalette(mon_palette_indexes->data[i]));
		pokemon_back[i]->SetPalette(GetPalette(mon_palette_indexes->data[i]));

		pokemon_leveling[i] = ReadFile(ResourceCache::GetResourceLocation(string("pokemon/leveling/").append(itos(i)).append(".dat")));
	}

#ifdef _DEBUG
	cout << "Done\n";
#endif
}

void ResourceCache::LoadMoves()
{
#ifdef _DEBUG
	cout << "--Loading moves...";
#endif
	DataBlock* d = ReadFile(ResourceCache::GetResourceLocation(string("moves/names.dat")).c_str());
	unsigned char* p = d->data;
	for (int i = 0; i < 256; i++)
	{
		string s;
		while ((unsigned int)(p - d->data_start) < d->size && *p != MESSAGE_ENDNAME)
			s.insert(s.begin() + s.length(), (char)*p++);
		p++;
		move_names[i] = s;
		//these move names get reported as a memory leak
		//however since theyre static not manually allocated, they will be deleted when the program terminates
		//the leak reportings are false
	}
	delete d;

	move_data = ReadFile(ResourceCache::GetResourceLocation(string("moves/moves.dat")).c_str());

#ifdef _DEBUG
	cout << "Done\n";
#endif
}