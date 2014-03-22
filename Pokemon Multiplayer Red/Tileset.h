#pragma once

#include "TileMap.h"
#include "ResourceCache.h"
#include "Utils.h"
#include "Constants.h"

class Tileset : public TileMap
{
public:
	Tileset(unsigned char index);
	~Tileset();

	void Draw(sf::RenderWindow* window, int dest_x, int dest_y, unsigned int tile, unsigned int tile_size_x, unsigned int tile_size_y);

	void AnimateWater();

protected:
	sf::Texture water_tile;
	sf::Sprite water8x8;
	DataBlock* misc_data;

	unsigned char water_animation_stage;
};