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
	inline DataBlock* GetMiscData() { return misc_data; }
	unsigned char GetTile8x8(unsigned char tile, unsigned char corner4x4);
	inline PaletteTexture* GetTransparentTiles() { return &transparent_tiles; }
	inline PaletteTexture* GetPoisonTiles() { return &poison_tiles; }
	inline void SetPoisonTimer() { poison_timer = 3; }

	bool IsDoorTile(unsigned char tile);

protected:
	DataBlock* misc_data;
	DataBlock* collision_data;
	DataBlock* door_tiles;

	PaletteTexture water_tile;
	PaletteTexture transparent_tiles; //transparent texture for drawing grass overlays
	PaletteTexture poison_tiles; //texture with a different color for poison. optimize performance at the cost of an extra 24kb per tileset... unless we used shaders
	sf::Sprite water8x8;
	sf::Sprite flower8x8;
	unsigned char water_animation_stage;
	unsigned char grass_tile;
	unsigned char poison_timer;
};
