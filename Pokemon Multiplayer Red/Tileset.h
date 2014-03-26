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

protected:
	PaletteTexture water_tile;
	sf::Sprite water8x8;
	sf::Sprite flower8x8;
	DataBlock* misc_data;

	unsigned char water_animation_stage;
};