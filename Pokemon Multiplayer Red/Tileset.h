#pragma once

#include "TileMap.h"
#include "ResourceCache.h"
#include "Utils.h"
#include "Constants.h"

class Tileset : public TileMap
{
public:
	Tileset(unsigned char index, sf::Texture* water_tile);
	~Tileset();

	void Draw(sf::RenderWindow* window, int dest_x, int dest_y, unsigned int tile, unsigned int tile_size_x, unsigned int tile_size_y);

	void AnimateWater();
	sf::Texture* water_tile;
	sf::Sprite water8x8;

	unsigned char water_animation_stage;
};