#include "Textbox.h"

Textbox::Textbox(unsigned char x, unsigned char y, unsigned char width, unsigned char height)
{
	tiles = 0;
	SetFrame(x, y, width, height);
}

Textbox::~Textbox()
{
	if (delete_items)
	{
		for (unsigned int i = 0; i < items.size(); i++)
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
	//try to move the cursor
	if (is_menu && (menu_flags & MenuFlags::FOCUSABLE) && (arrow_state & ArrowStates::ACTIVE))
	{
		//this menu is active, so try to move the cursor up
		if (InputController::KeyDownOnce(INPUT_UP))
		{
			if (active_index > 0)
			{
				active_index--;
				if (GetScrollIndex() < 0)
					scroll_start--;
			}
			else if (menu_flags & MenuFlags::WRAPS)
			{
				active_index = items.size() - 1;
			}
		}
		else if (InputController::KeyDownOnce(INPUT_DOWN)) //try down now
		{
			if (active_index < items.size() - 1)
			{
				active_index++;
				//if (GetScrollIndex() >= scroll_start)
				//	scroll_start++;
			}
			else if (menu_flags & MenuFlags::WRAPS)
			{
				active_index = 0;
			}
		}
	}
}

void Textbox::Render(sf::RenderWindow* window)
{
	DrawFrame(window);
	if (is_menu && (menu_flags & MenuFlags::FOCUSABLE))
	{
		if (arrow_state & ArrowStates::ACTIVE)
			DrawArrow(window, true);
		if (arrow_state & ArrowStates::INACTIVE)
			DrawArrow(window, false);
	}
}

void Textbox::SetFrame(unsigned char x, unsigned char y, unsigned char width, unsigned char height)
{
	pos = sf::Vector2i(x, y);
	size = sf::Vector2u(width, height);
	if (tiles)
		delete[] tiles;
	tiles = new unsigned char[(width - 2) * (height - 2)];
	for (int i = 0; i < (width - 2) * (height - 2); i++)
		tiles[i] = MENU_BLANK;
}

void Textbox::SetMenu(bool menu, unsigned char display_count, sf::Vector2i start, sf::Vector2u spacing, unsigned int flags, bool delete_items)
{
	this->is_menu = menu;
	this->display_count = display_count;
	this->delete_items = delete_items;
	this->item_start = start;
	this->item_spacing = spacing;
	this->menu_flags = flags;
	if (this->delete_items)
	{
		for (unsigned int i = 0; i < items.size(); i++)
		{
			if (items[i])
				delete items[i];
		}
	}
	this->delete_items = delete_items;
	this->items.resize(0);
	UpdateMenu();
}

void Textbox::DrawFrame(sf::RenderWindow* window)
{
	PaletteTexture* frame = ResourceCache::GetMenuTexture();
	sf::IntRect src_rect(0, 0, 8, 8);
	for (int x = pos.x; x < (int)(pos.x + size.x); x++)
	{
		for (int y = pos.y; y < (int)(pos.y + size.y); y++)
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
			sprite8x8.setPosition((float)(x * 8), (float)(y * 8));

			window->draw(sprite8x8);
		}
	}
}

void Textbox::UpdateMenu()
{
	//this function creates "tiles" for display
	for (unsigned int i = scroll_start; i < scroll_start + display_count && i < items.size(); i++)
	{
		TextItem* item = items[i];
		if (!item)
			continue;
		unsigned int x = item_start.x;
		unsigned int y = item_start.y + (i - scroll_start) * item_spacing.y;
		for (unsigned int c = 0; c < item->text.length(); c++)
		{
			tiles[x + c + y * (size.x - 2)] = item->text[c];
		}
	}
}

void Textbox::DrawArrow(sf::RenderWindow* window, bool active)
{
	sf::IntRect src_rect = sf::IntRect(0, 0, 8, 8);
	sprite8x8.setTexture(*ResourceCache::GetFontTexture());

	src_rect.left = ((active ? CURSOR_ACTIVE : CURSOR_INACTIVE) % 16) * 8;
	src_rect.top = ((active ? CURSOR_ACTIVE : CURSOR_INACTIVE) / 16) * 8;
	unsigned int index = active_index - scroll_start;
	sprite8x8.setPosition((float)(pos.x * 8 + item_start.x * 8), (float)(pos.y * 8 + item_start.y * 8 + 8 + index * 8 * item_spacing.y));
	sprite8x8.setTextureRect(src_rect);
	window->draw(sprite8x8);
}