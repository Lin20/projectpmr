#pragma once

#include "Common.h"
#include "TileMap.h"
#include "Utils.h"
#include "ResourceCache.h"
#include "DataBlock.h"
#include "MapConnection.h"

class Map
{
public:
	Map(unsigned char index);
	~Map();

	bool Load();

	unsigned char index;
	unsigned char width;
	unsigned char height;
	unsigned char tileset;
	unsigned char border_tile;

	unsigned char* tiles;
	MapConnection connections[4];

	inline bool HasConnection(unsigned char e) { return (connection_mask & (1 << (3 - e))) != 0; }

private:
	unsigned char connection_mask;
	bool ParseHeader(DataBlock& data);
};

