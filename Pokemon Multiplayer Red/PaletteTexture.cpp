#include "PaletteTexture.h"


PaletteTexture::PaletteTexture(const char* filename)
{
	pixels = 0;
	palette[0] = DEFAULT_PALETTE_0;
	palette[1] = DEFAULT_PALETTE_1;
	palette[2] = DEFAULT_PALETTE_2;
	palette[3] = DEFAULT_PALETTE_3;
	if (filename)
	{
		loadFromFile(filename);
	}
}

PaletteTexture::~PaletteTexture()
{
	if (pixels)
		delete[] pixels;
}

bool PaletteTexture::loadFromFile(const std::string& filename)
{
	sf::Image temp;
	if (!temp.loadFromFile(filename))
		return false;
	underlying_texture.loadFromImage(temp);
	size = temp.getSize();
	pixels = new sf::Uint8[size.x * size.y * 4];
	memcpy(pixels, temp.getPixelsPtr(), size.x * size.y * 4);
	return true;
}

void PaletteTexture::SetPalette(const sf::Color new_palette[])
{
	if ((size.x | size.y) == 0 || !pixels)
		return;
	sf::Uint32 pal0 = ((palette[0].r / 8 * 8) << 24) + ((palette[0].g / 8 * 8) << 16) + ((palette[0].b / 8 * 8) << 8) + palette[0].a;
	sf::Uint32 pal1 = ((palette[1].r / 8 * 8) << 24) + ((palette[1].g / 8 * 8) << 16) + ((palette[1].b / 8 * 8) << 8) + palette[1].a;
	sf::Uint32 pal2 = ((palette[2].r / 8 * 8) << 24) + ((palette[2].g / 8 * 8) << 16) + ((palette[2].b / 8 * 8) << 8) + palette[2].a;
	sf::Uint32 pal3 = ((palette[3].r / 8 * 8) << 24) + ((palette[3].g / 8 * 8) << 16) + ((palette[3].b / 8 * 8) << 8) + palette[3].a;
	sf::Uint32 new0 = ((new_palette[0].r / 8 * 8) << 24) + ((new_palette[0].g / 8 * 8) << 16) + ((new_palette[0].b / 8 * 8) << 8) + new_palette[0].a;
	sf::Uint32 new1 = ((new_palette[1].r / 8 * 8) << 24) + ((new_palette[1].g / 8 * 8) << 16) + ((new_palette[1].b / 8 * 8) << 8) + new_palette[1].a;
	sf::Uint32 new2 = ((new_palette[2].r / 8 * 8) << 24) + ((new_palette[2].g / 8 * 8) << 16) + ((new_palette[2].b / 8 * 8) << 8) + new_palette[2].a;
	sf::Uint32 new3 = ((new_palette[3].r / 8 * 8) << 24) + ((new_palette[3].g / 8 * 8) << 16) + ((new_palette[3].b / 8 * 8) << 8) + new_palette[3].a;
	for (unsigned int x = 0; x < size.x; x++)
	{
		for (unsigned int y = 0; y < size.y; y++)
		{
			sf::Uint32 p = ((pixels[(x + y * size.x) * 4] / 8 * 8) << 24) + ((pixels[(x + y * size.x) * 4 + 1] / 8 * 8) << 16) + ((pixels[(x + y * size.x) * 4 + 2] / 8 * 8) << 8) + (pixels[(x + y * size.x) * 4 + 3] / 8 * 8);
			if ((p & 0xFFFFFF00) == (pal3 & 0xFFFFFF00))
			{
				pixels[(x + y * size.x) * 4] = (new3 >> 24) & 0xFF;
				pixels[(x + y * size.x) * 4 + 1] = (new3 >> 16) & 0xFF;
				pixels[(x + y * size.x) * 4 + 2] = (new3 >> 8) & 0xFF;
				pixels[(x + y * size.x) * 4 + 3] = new3 & 0xFF;
			}
			else if ((p & 0xFFFFFF00) == (pal1 & 0xFFFFFF00))
			{
				pixels[(x + y * size.x) * 4] = (new1 >> 24) & 0xFF;
				pixels[(x + y * size.x) * 4 + 1] = (new1 >> 16) & 0xFF;
				pixels[(x + y * size.x) * 4 + 2] = (new1 >> 8) & 0xFF;
				pixels[(x + y * size.x) * 4 + 3] = new1 & 0xFF;
			}
			else if ((p & 0xFFFFFF00) == (pal2 & 0xFFFFFF00))
			{
				pixels[(x + y * size.x) * 4] = (new2 >> 24) & 0xFF;
				pixels[(x + y * size.x) * 4 + 1] = (new2 >> 16) & 0xFF;
				pixels[(x + y * size.x) * 4 + 2] = (new2 >> 8) & 0xFF;
				pixels[(x + y * size.x) * 4 + 3] = new2 & 0xFF;
			}
			else
			{
				pixels[(x + y * size.x) * 4] = (new0 >> 24) & 0xFF;
				pixels[(x + y * size.x) * 4 + 1] = (new0 >> 16) & 0xFF;
				pixels[(x + y * size.x) * 4 + 2] = (new0 >> 8) & 0xFF;
				pixels[(x + y * size.x) * 4 + 3] = new0 & 0xFF;
			}
		}
	}

	memcpy(palette, new_palette, sizeof(sf::Color) * 4);
	underlying_texture.update(pixels);
}