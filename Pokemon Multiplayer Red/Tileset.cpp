#include "Tileset.h"

Tileset::Tileset(unsigned char index) : TileMap()
{
	Load(index);
}

Tileset::~Tileset()
{
	if (misc_data)
		delete misc_data;
}

void Tileset::Load(unsigned char index)
{
	tiles_tex = new PaletteTexture();
	tiles_tex->loadFromFile(ResourceCache::GetResourceLocation(string("tilesets\\").append(to_string(index)).append(".png")));
	water_tile.loadFromFile(ResourceCache::GetResourceLocation(string("tilesets\\water\\").append(to_string(index)).append(".png")));
	formation = ReadFile(ResourceCache::GetResourceLocation(string("tilesets\\formation\\").append(to_string(index)).append(".dat")).c_str());
	misc_data = ReadFile(ResourceCache::GetResourceLocation(string("tilesets\\misc\\").append(to_string(index)).append(".dat")).c_str());

	tiles_x = 16;
	this->index = index;
	delete_texture = true;

	sprite8x8.setTexture(tiles_tex->GetTexture());
	water8x8.setTexture(water_tile.GetTexture());
	flower8x8.setTexture(*ResourceCache::GetFlowerTexture());
}

void Tileset::Draw(sf::RenderWindow* window, int dest_x, int dest_y, unsigned int tile, unsigned int tile_size_x, unsigned int tile_size_y)
{
	if (!tiles_tex)
		return;
	sf::IntRect src_rect = sf::IntRect(0, 0, 8, 8);
	sf::Sprite* sprite = &sprite8x8;

	for (unsigned int y = 0; y < tile_size_y; y++)
	{
		for (unsigned int x = 0; x < tile_size_x; x++)
		{
			unsigned char t = (formation ? formation->data[tile * tile_size_x * tile_size_y + y * tile_size_x + x] : tile * tile_size_x * tile_size_y + y * tile_size_x + x);
			if (t == WATER_TILE && this->misc_data->data[4])
			{
				sprite = &water8x8;
				src_rect.left = (water_animation_stage / ANIMATION_TIMER) * 8;
				if (water_animation_stage / ANIMATION_TIMER > 3)
					src_rect.left = (8 - (water_animation_stage / ANIMATION_TIMER)) * 8;
				src_rect.top = 0;
			}
			else if (t == FLOWER_TILE && this->misc_data->data[4] & 2)
			{
				sprite = &flower8x8;
				src_rect.left = ((water_animation_stage / ANIMATION_TIMER) % 4 - 1) * 8;
				if (src_rect.left < 0)
					src_rect.left = 0;
				src_rect.top = 0;
			}
			else
			{
				sprite = &sprite8x8;
				src_rect.left = (t % tiles_x) * 8;
				src_rect.top = (t / tiles_x) * 8;
			}
			sprite->setTextureRect(src_rect);
			sprite->setPosition((float)(int)(dest_x * 8 * (int)tile_size_x + x * 8), (float)(int)(dest_y * 8 * (int)tile_size_y + y * 8));
			window->draw(*sprite);
		}
	}
}

void Tileset::AnimateWater()
{
	water_animation_stage++;
	if (water_animation_stage >= ANIMATION_TIMER * 8)
		water_animation_stage = 0;
}

void Tileset::SetPalette(sf::Color palette[])
{
	water_tile.SetPalette(palette);
	tiles_tex->SetPalette(palette);
	ResourceCache::GetFlowerTexture()->SetPalette(palette);
}