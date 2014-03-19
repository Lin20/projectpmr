#include "Tileset.h"

Tileset::Tileset(unsigned char index) : TileMap()
{
	tiles_tex = new sf::Texture();
	tiles_tex->loadFromFile(ResourceCache::GetResourceLocation(string("tilesets\\").append(to_string(index)).append(".png")));
 	formation = ReadFile(ResourceCache::GetResourceLocation(string("tilesets\\formation\\").append(to_string(index)).append(".dat")).c_str());
	tiles_x = 16;
	this->index = index;
	delete_texture = true;
	sprite8x8.setTexture(*tiles_tex);
}

Tileset::~Tileset()
{
	
}