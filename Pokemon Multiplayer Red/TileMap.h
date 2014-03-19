#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/Graphics/Texture.hpp>

class TileMap
{
public:
	TileMap(sf::Texture* tiles_texture, unsigned char* formation, unsigned int t_x, unsigned char index = 0, bool delete_tex = false);
	~TileMap();

	void Draw(sf::RenderWindow* window, int dest_x, int dest_y, unsigned int tile, unsigned int tile_size_x, unsigned int tile_size_y);

private:
	unsigned char index;
	unsigned char* formation;
	unsigned char tiles_x;
	sf::Texture* tiles_tex;
	bool delete_texture;
	sf::Sprite sprite8x8;
};

