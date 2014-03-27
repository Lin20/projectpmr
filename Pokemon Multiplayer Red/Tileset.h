#pragma once

#include "TileMap.h"
#include "ResourceCache.h"
#include "Utils.h"
#include "Constants.h"
#include "PaletteTexture.h"

class Tileset : public TileMap
{
public:
	Tileset(unsigned char index);
	~Tileset();

	void Load(unsigned char index);

	void Render(sf::RenderWindow* window, int dest_x, int dest_y, unsigned int tile, unsigned int tile_size_x, unsigned int tile_size_y);

	void AnimateTiles();
	void SetPalette(sf::Color palette[]);

	inline DataBlock* GetCollisionData() { return collision_data; }
	unsigned char GetTile8x8(unsigned char tile, unsigned char corner4x4);

protected:
	DataBlock* misc_data;
	DataBlock* collision_data;

	PaletteTexture water_tile;
	sf::Sprite water8x8;
	sf::Sprite flower8x8;
	unsigned char water_animation_stage;
	unsigned char grass_tile;
};