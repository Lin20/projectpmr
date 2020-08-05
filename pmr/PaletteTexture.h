#pragma once

#include <cstring>
#include <SFML/Graphics.hpp>
#include "Constants.h"

class PaletteTexture
{
public:
	PaletteTexture(const char* filename = 0);
	~PaletteTexture();

	bool loadFromFile(const std::string& filename);
	void Copy(PaletteTexture* src);
	void As8x8Tile(PaletteTexture* from, int tile);
	void SetPalette(const sf::Color new_palette[]);

	//Allow implicit casting to sf::Texture so we don't need to use GetTexture() all the time
	operator sf::Texture&() { return underlying_texture; }

	sf::Texture* GetTexture() { return &underlying_texture; }
	sf::Uint8* GetPixels() { return pixels; }
	sf::Uint8* GetOriginalPixels() { return original_pixels; }
	sf::Color* GetPalette() { return palette; }

private:
	sf::Texture underlying_texture;
	sf::Color palette[4];
	sf::Uint8* pixels;
	sf::Uint8* original_pixels;
	sf::Vector2u size;
};

