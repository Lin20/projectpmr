#include "Map.h"

Map::Map()
{
}

Map::Map(unsigned char index)
{
	this->index = index;
	tiles = 0;
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
	return true;
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
		}
	}

	border_tile = *p++;
	return true;
}