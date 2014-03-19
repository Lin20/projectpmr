#pragma once

#include "TileMap.h"
#include "ResourceCache.h"
#include "Utils.h"

class Tileset : public TileMap
{
public:
	Tileset(unsigned char index);
	~Tileset();
};