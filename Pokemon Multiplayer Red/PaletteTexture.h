#pragma once

#include <SFML/Graphics.hpp>
#include "Constants.h"

class PaletteTexture
{
public:
	PaletteTexture(const char* filename = 0);
	~PaletteTexture();

	sf::Texture& GetTexture() { return underlying_texture; }
	void SetPalette(const sf::Color new_palette[]);

private:
	sf::Texture underlying_texture;
	sf::Color palette[4];
	sf::Uint8* pixels;
	sf::Vector2u size;
};

