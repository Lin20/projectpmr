#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/Graphics/Texture.hpp>
#include "DataBlock.h"
#include "PaletteTexture.h"

class TileMap
{
public:
	TileMap(PaletteTexture* tiles_texture = 0, DataBlock* formation = 0, unsigned int t_x = 0, unsigned char index = 0, bool delete_tex = false);
	virtual ~TileMap();

	void Render(sf::RenderWindow* window, int dest_x, int dest_y, unsigned int tile, unsigned int tile_size_x, unsigned int tile_size_y);
	inline PaletteTexture* GetTexture() { return tiles_tex; }

protected:
	unsigned char index;
	DataBlock* formation;
	unsigned char tiles_x;
	PaletteTexture* tiles_tex;
	bool delete_texture;
	sf::Sprite sprite8x8;
};