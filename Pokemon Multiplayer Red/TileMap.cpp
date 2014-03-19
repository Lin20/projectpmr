#include "TileMap.h"

TileMap::TileMap(sf::Texture* tiles_texture, DataBlock* formation, unsigned int t_x, unsigned char index, bool delete_tex)
{
	tiles_tex = tiles_texture;
	delete_texture = false;
	this->formation = formation;
	this->tiles_x = t_x;
	this->index = index;
	this->delete_texture = delete_tex;
	if (tiles_tex)
		sprite8x8.setTexture(*tiles_tex, true);
}

TileMap::~TileMap()
{
	if (formation)
		delete formation;
}

void TileMap::Draw(sf::RenderWindow* window, int dest_x, int dest_y, unsigned int tile, unsigned int tile_size_x, unsigned int tile_size_y)
{
	if (!tiles_tex)
		return;
	sf::IntRect src_rect = sf::IntRect(0, 0, 8, 8);
	for (unsigned int y = 0; y < tile_size_y; y++)
	{
		for (unsigned int x = 0; x < tile_size_x; x++)
		{
			unsigned char t = (formation ? formation->data[tile * tile_size_x * tile_size_y + y * tile_size_x + x] : tile * tile_size_x * tile_size_y + y * tile_size_x + x);
			src_rect.left = (t % tiles_x) * 8;
			src_rect.top = (t / tiles_x) * 8;
			sprite8x8.setTextureRect(src_rect);
			sprite8x8.setPosition((float)(dest_x * 8 * tile_size_x + x * 8), (float)(dest_y * 8 * tile_size_y + y * 8));
			window->draw(sprite8x8);
		}
	}
}