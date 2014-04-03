#include "Textbox.h"


Textbox::Textbox(unsigned char x, unsigned char y, unsigned char width, unsigned char height)
{
	SetFrame(x, y, width, height);
}

Textbox::~Textbox()
{
	if (delete_items)
	{
		for (int i = 0; i < items.size(); i++)
		{
			if (items[i])
				delete items[i];
		}
	}
	if (tiles)
		delete tiles;
}

void Textbox::Update()
{

}

void Textbox::Render(sf::RenderWindow* window)
{
	DrawFrame(window);
	if (is_menu)
		DrawMenu(window);
}

void Textbox::SetFrame(unsigned char x, unsigned char y, unsigned char width, unsigned char height)
{
	pos = sf::Vector2u(x, y);
	size = sf::Vector2u(width, height);
	tiles = new unsigned char[(width - 2) * (height - 2)];
	for (int i = 0; i < (width - 2) * (height - 2); i++)
		tiles[i] = MENU_BLANK;
}

void Textbox::SetMenu(bool menu, unsigned char display_count, sf::Vector2u start, sf::Vector2u spacing, bool delete_items)
{
	this->is_menu = menu;
	this->display_count = display_count;
	this->delete_items = delete_items;
	this->item_start = start;
	this->item_spacing = spacing;
	if (this->delete_items)
	{
		for (int i = 0; i < items.size(); i++)
		{
			if (items[i])
				delete items[i];
		}
	}
	this->items.resize(0);
	UpdateMenu();
}

void Textbox::DrawFrame(sf::RenderWindow* window)
{
	PaletteTexture* frame = ResourceCache::GetMenuTexture();
	sf::IntRect src_rect(0, 0, 8, 8);
	for (int x = pos.x; x < pos.x + size.x; x++)
	{
		for (int y = pos.y; y < pos.y + size.y; y++)
		{
			//determine which frame tile to draw
			sprite8x8.setTexture(*frame);
			unsigned char tile = MENU_BLANK;
			if (x == pos.x && y == pos.y)
				tile = MENU_CORNER_UL;
			else if (x == pos.x + size.x - 1 && y == pos.y)
				tile = MENU_CORNER_UR;
			else if (x == pos.x && y == pos.y + size.y - 1)
				tile = MENU_CORNER_DL;
			else if (x == pos.x + size.x - 1 && y == pos.y + size.y - 1)
				tile = MENU_CORNER_DR;
			else if (y == pos.y || y == pos.y + size.y - 1)
				tile = MENU_H;
			else if (x == pos.x || x == pos.x + size.x - 1)
				tile = MENU_V;
			else
			{
				tile = (unsigned char)tiles[(x - pos.x - 1) + (y - pos.y - 1) * (size.x - 2)];
				if (tile >= 0x80) //use the font texture
					sprite8x8.setTexture(*ResourceCache::GetFontTexture());
				tile &= 0x7F;
			}

			src_rect.left = (tile % 16) * 8;
			src_rect.top = (tile / 16) * 8;
			sprite8x8.setTextureRect(src_rect);
			sprite8x8.setPosition(x * 8, y * 8);

			window->draw(sprite8x8);
		}
	}
}

void Textbox::DrawMenu(sf::RenderWindow* window)
{

}

void Textbox::UpdateMenu()
{
	//this function creates "tiles" for display
	for (int i = scroll_start; i < scroll_start + display_count && i < items.size(); i++)
	{
		TextItem* item = items[i];
		if (!item)
			continue;
		unsigned int x = item_start.x;
		unsigned int y = item_start.y + (i - scroll_start) * item_spacing.y;
		for (int c = 0; c < item->text.length(); c++)
		{
			tiles[x + c + y * (size.x - 2)] = item->text[c];
		}
	}
}