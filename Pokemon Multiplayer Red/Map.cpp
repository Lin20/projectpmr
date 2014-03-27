#include "Map.h"

Map::Map()
{
}

Map::Map(unsigned char index)
{
	this->index = index;
	tiles = 0;
	palette = ResourceCache::GetPalette(0);
}

Map::~Map()
{
	if (tiles)
		delete[] tiles;
	for (int i = 0; i < 4; i++)
	{
		if (connected_maps[i])
			delete connected_maps[i];
	}
}

bool Map::Load(bool only_load_tiles)
{
	DataBlock* data = ReadFile(ResourceCache::GetResourceLocation(string("maps\\").append(to_string(index).append(".dat"))).c_str());
	if (!ParseHeader(data, only_load_tiles))
		return false;
	LoadPalette();
	return true;
}

void Map::LoadPalette()
{
	unsigned char pal_index = ResourceCache::GetMapPaletteIndex(index);
	if (pal_index != 0xFF)
		palette = ResourceCache::GetPalette(pal_index);
}

bool Map::ParseHeader(DataBlock* data, bool only_load_tiles)
{
	if (!data || data->size < 3)
		return false;

	unsigned char* p = data->data;
	tileset = *p++;
	height = *p++;
	width = *p++;

	tiles = new unsigned char[width * height];
	if (data->size - 3 < (unsigned int)(width * height))
		return true;
	memcpy(tiles, p, width * height);
	if (only_load_tiles)
		return true;
	p += width * height;

	connection_mask = *p++;
	for (int b = 3; b >= 0; b--)
	{
		if ((connection_mask & (1 << b)) != 0)
		{
			connections[3 - b].map = *p++;
			connections[3 - b].y_alignment = *p++;
			connections[3 - b].x_alignment = *p++;
		}
	}
	for (int i = 0; i < 4; i++)
	{
		if (HasConnection(i))
		{
			if (!connected_maps[i])
				connected_maps[i] = new Map(connections[i].map);
			else
				connected_maps[i]->index = connections[i].map;
			connected_maps[i]->Load(true);
			if (connected_maps[i]->width * connected_maps[i]->height >= 128 * 128) //bad map
			{
				connected_maps[i] = 0;
				connection_mask ^= 1 << (3 - i);
			}
		}
	}

	border_tile = *p++;
	return true;
}

bool Map::IsPassable(int x, int y)
{
	if (x < 0 || y < 0 || x >= width * 2 || y >= height * 2)
		return false;
	Tileset* tileset = ResourceCache::GetTileset(this->tileset);
	if (!tileset)
		return true;
	DataBlock* collision = tileset->GetCollisionData();
	if (!collision)
		return true;

	//the original game uses the lower-left tile of a 16x16 block to determine whether or not that block is passable
	unsigned char tile = tileset->GetTile8x8(tiles[x / 2 + y / 2 * width], (x % 2 == 0 ? 0 : 2) + (y % 2 == 0 ? 4 : 12));
	for (int i = 0; i < collision->size; i++)
	{
		if (collision->data[i] == tile)
			return true;
	}

	return false;
}

bool Map::CanJump(int x, int y, unsigned char direction)
{
	if (x < 0 || y < 0 || x >= width * 2 || y >= height * 2)
		return false;
	if (this->tileset > 0)
		return false;
	Tileset* tileset = ResourceCache::GetTileset(this->tileset);
	if (!tileset)
		return true;
	DataBlock* collision = tileset->GetCollisionData();
	if (!collision)
		return true;

	//the original game uses the lower-left tile of a 16x16 block to determine whether or not that block is jumpable
	unsigned char standing_on = tileset->GetTile8x8(tiles[x / 2 + y / 2 * width], (x % 2 == 0 ? 0 : 2) + (y % 2 == 0 ? 4 : 12));
	x += DELTAX(direction);
	y += DELTAY(direction);
	unsigned char next = tileset->GetTile8x8(tiles[x / 2 + y / 2 * width], (x % 2 == 0 ? 0 : 2) + (y % 2 == 0 ? 4 : 12));

	unsigned char* p = ResourceCache::GetLedges()->data;
	while (p < ResourceCache::GetLedges()->data + ResourceCache::GetLedges()->size)
	{
		if (*p == 0xFF)
			return false;
		if (*p++ / 4 != direction)
		{
			p += 3;
			continue;
		}
		if (*p++ != standing_on)
		{
			p += 2;
			continue;
		}
		if (*p++ != next)
		{
			p++;
			continue;
		}
		p++; //button input
		return true;
	}

	return false;
}