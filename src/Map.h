#pragma once

#include <cstring>
#include "Common.h"
#include "TileMap.h"
#include "Utils.h"
#include "ResourceCache.h"
#include "DataBlock.h"
#include "MapConnection.h"
#include "Events.h"
#include "OverworldEntity.h"

class Map
{
public:
	Map();
	Map(unsigned char index, vector<OverworldEntity*>* scene_entities);
	~Map();

	bool Load(bool only_load_tiles = false);
	void LoadPalette();

	unsigned char index;
	unsigned char width;
	unsigned char height;
	unsigned char tileset;
	unsigned char border_tile;

	unsigned char* tiles;
	MapConnection connections[4];
	Map* connected_maps[4];
	std::vector<Warp> warps;
	std::vector<Sign> signs;
	std::vector<Entity> entities;

	inline bool HasConnection(unsigned char e) { return (connection_mask & (1 << (3 - e))) != 0; }
	inline sf::Color* GetPalette() { return palette; }
	unsigned char Get8x8Tile(int x, int y);
	unsigned char GetCornerTile(int x, int y, unsigned char corner);
	bool IsPassable(int x, int y, OverworldEntity* ignore = 0);
	bool CanJump(int x, int y, unsigned char direction); //can jump from the current position facing the specified direction
	bool InGrass(int x, int y);
	bool CanWarp(int x, int y, unsigned char direction, Warp* check_warp);

	void RenderRectangle(int x, int y, int width, int height, sf::Sprite& sprite, sf::RenderWindow* window);

	Warp GetWarp(unsigned int index)
	{
		if (index < warps.size())
			return warps[index];
		return Warp();
	}

	Warp* GetWarpAt(int x, int y)
	{
		for (unsigned int i = 0; i < warps.size(); i++)
		{
			if ((int)warps[i].x == x && (int)warps[i].y == y)
				return &warps[i];
		}
		return 0;
	}

private:
	unsigned char connection_mask;
	bool ParseHeader(DataBlock* data, bool only_load_tiles = false);
	sf::Color* palette;
	vector<OverworldEntity*>* scene_entities;
};

