#pragma once

#include <SFML/Graphics.hpp>
#include "Constants.h"

class PaletteTexture
{
public:
	PaletteTexture(const char* filename = 0);
	~PaletteTexture();

	bool loadFromFile(const std::string& filename);
	void SetPalette(const sf::Color new_palette[]);

	//Allow implicit casting to sf::Texture so we don't need to use GetTexture() all the time
	operator sf::Texture&() { return underlying_texture; }

	sf::Texture& GetTexture() { return underlying_texture; }

private:
	sf::Texture underlying_texture;
	sf::Color palette[4];
	sf::Uint8* pixels;
	sf::Vector2u size;
};

