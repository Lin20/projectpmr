#include "Textbox.h"

Textbox::Textbox(char x, char y, unsigned char width, unsigned char height, bool d, bool hidden_frame)
{
	tiles = 0;
	delete_on_close = d;
	close = false;
	cancel_switch = false;
	close_when_no_children = false;
	hide_frame = hidden_frame;

	is_menu = false;
	display_count = 0;
	scroll_pos = 0;
	scroll_start = INT_MAX;
	close_callback = nullptr;

	active_index = 0;
	inactive_index = 0;
	menu_flags = 0;
	arrow_state = 0;
	menu_open_delay = 0;

	text = 0;
	text_tile_pos = 0;
	text_pos = 0;
	autoscroll = false;
	text_timer = 0;
	arrow_timer = 0;
	text_speed = TEXT_SPEED_SLOW;
	scroll_timer = 0;
	cursor_visibility_timer = 0;
	auto_close_timer = 0;

	SetFrame(x, y, width, height);
}

Textbox::~Textbox()
{
	//if (delete_on_close)
	{
		if (text)
			delete text;

		for (unsigned int i = 0; i < items.size(); i++)
		{
			if (items[i])
				delete items[i];
		}
	}
	if (tiles)
	{
		delete[] tiles;
		tiles = 0;
	}
}

//TODO: Clean up a bit. Split into functions.
void Textbox::Update()
{
	if (UpdateTextboxes())
		return;
	if (close_when_no_children)
		close = true;
	if (arrow_timer > 0)
		arrow_timer--;
	if (cursor_visibility_timer > 0)
		cursor_visibility_timer--;
	if (menu_open_delay > 0)
		return;
	//try to move the cursor
	if (is_menu && (menu_flags & MenuFlags::FOCUSABLE) && (arrow_state & ArrowStates::ACTIVE))
	{
		//this menu is active, so try to move the cursor up
		if (InputController::KeyDownOnce(INPUT_UP))
		{
			if (GetScrollIndex() - 1 < 0 && cursor_visibility_timer == 0 && scroll_start < display_count)
				cursor_visibility_timer = CURSOR_VIS_TIME;
			scroll_timer = 0;
			Scroll(true);
		}
		else if (InputController::KeyDown(INPUT_UP) && (menu_flags & MenuFlags::HOLD_INPUT)) //hold down
		{
			scroll_timer++;
			if (scroll_timer >= 36 && scroll_timer % 6 == 0)
			{
				if (cursor_visibility_timer == 0)
					cursor_visibility_timer = CURSOR_VIS_TIME;
				Scroll(true);
			}
		}
		else if (InputController::KeyDownOnce(INPUT_DOWN)) //try down now
		{
			if (GetScrollIndex() + 1 > (int)scroll_start)
				if (cursor_visibility_timer == 0)
					cursor_visibility_timer = CURSOR_VIS_TIME;
			scroll_timer = 0;
			Scroll(false);
		}
		else if (InputController::KeyDown(INPUT_DOWN) && (menu_flags & MenuFlags::HOLD_INPUT)) //hold down
		{
			scroll_timer++;
			if (scroll_timer >= 36 && scroll_timer % 6 == 0)
			{
				if (cursor_visibility_timer == 0)
					cursor_visibility_timer = CURSOR_VIS_TIME;
				Scroll(false);
			}
		}
		else if (InputController::KeyDownOnce(INPUT_SELECT)) //press select
		{
			if (cursor_visibility_timer == 0)
				cursor_visibility_timer = CURSOR_VIS_TIME;
			if (switch_last_item || (!switch_last_item && active_index < items.size() - 1))
			{
				if ((menu_flags & MenuFlags::SWITCHABLE) && (arrow_state & ArrowStates::ACTIVE) && (arrow_state & ArrowStates::INACTIVE) && switch_callback != nullptr)
				{
					switch_callback();
					if (!cancel_switch)
					{
						TextItem* t = items[active_index];
						items[active_index] = items[inactive_index];
						items[inactive_index] = t;
						SetArrowState(ArrowStates::ACTIVE);
						UpdateMenu();
					}
					cancel_switch = false;
				}
				else if (menu_flags & MenuFlags::SWITCHABLE)
				{
					SetArrowState(ArrowStates::ACTIVE | ArrowStates::INACTIVE);
					inactive_index = active_index;
				}
			}
		}
		else if (InputController::KeyDownOnce(INPUT_A)) //press a
		{
			if ((menu_flags & MenuFlags::SWITCHABLE) && (menu_flags & MenuFlags::A_TO_SWITCH) && (arrow_state & ArrowStates::ACTIVE) && (arrow_state & ArrowStates::INACTIVE) && switch_callback != nullptr)
			{
				switch_callback();
				if (!cancel_switch)
				{
					TextItem* t = items[active_index];
					items[active_index] = items[inactive_index];
					items[inactive_index] = t;
					SetArrowState(ArrowStates::ACTIVE);
					UpdateMenu();
				}
				cancel_switch = false;
			}
			else
				items[active_index]->Action();
		}
		else if (InputController::KeyDownOnce(INPUT_B)) //press b
		{
			if ((menu_flags & MenuFlags::SWITCHABLE) && (menu_flags & MenuFlags::A_TO_SWITCH) && (arrow_state & ArrowStates::ACTIVE) && (arrow_state & ArrowStates::INACTIVE) && switch_callback != nullptr)
			{
				CancelSwitch();
				SetArrowState(ArrowStates::ACTIVE);
				switch_callback();
			}
			else
				Close();
		}
		else
			scroll_timer = 0;
		if (scroll_pos + display_count < items.size() && arrow_timer == 0)
			arrow_timer = CURSOR_MORE_TIME;
	}
	else if (is_counter) //it's a counter
	{
		if (InputController::KeyDownOnce(INPUT_DOWN))
		{
			counter_value--;
			if (counter_value < min_counter)
				counter_value = max_counter;
			UpdateCounter();
		}
		else if (InputController::KeyDownOnce(INPUT_UP))
		{
			counter_value++;
			if (counter_value > max_counter)
				counter_value = min_counter;
			UpdateCounter();
		}
		else if (InputController::KeyDownOnce(INPUT_A))
		{
			if (counter_callback != nullptr)
				counter_callback();
		}
		else if (InputController::KeyDownOnce(INPUT_B))
		{
			if (counter_close_callback != nullptr)
				counter_close_callback();
			Close();
		}
	}
	else if (!is_menu && !is_counter) //it's a regular textbox
	{
		if (auto_close_timer > 0)
		{
			auto_close_timer++;
			if (auto_close_timer >= TEXT_AUTOCLOSE_MAX)
			{
				auto_close_timer = 0;
				Close();
				text->Action();
			}
		}
		else
		{
			if (text_timer > 0)
				text_timer--;
			if (!text_timer)
			{
				if (scroll_timer > 0)
				{
					if (scroll_timer / 4 > 0 && scroll_timer % 4 == 0)
					{
						memcpy(tiles, tiles + size.x - 2, (size.x - 2) * 3);
						memset(tiles + (size.x - 2) * 3, MENU_BLANK, (size.x - 2));
					}
					scroll_timer--;
					if (scroll_timer == 0)
						text_tile_pos = (size.x - 2) * 3;
				}
				else
				{
					for (int i = 0; i < text_speed && scroll_timer == 0 && text_timer < TEXT_TIMER_BLANK; i++)
						ProcessNextCharacter();
				}
			}
		}
	}
}

void Textbox::Render(sf::RenderWindow* window)
{
	//we have to move this here because only the last textbox in a list gets updated
	if (menu_open_delay > 0)
		menu_open_delay--;
	DrawFrame(window);
}

void Textbox::SetFrame(char x, char y, unsigned char width, unsigned char height)
{
	pos = sf::Vector2i(x, y);
	size = sf::Vector2u(width, height);
	if (tiles)
		delete[] tiles;
	tiles = new unsigned char[(width - 2) * (height - 1)];
	for (int i = 0; i < (width - 2) * (height - 1); i++)
		tiles[i] = MENU_BLANK;
}

void Textbox::SetText(TextItem* text)
{
	this->text = text;
	this->text_tile_pos = size.x - 2;
	this->text_pos = 0;
	this->text_timer = 0; //there's usually a delay before the textbox starts
}

void Textbox::SetText(string text)
{
	ClearItems();
	SetText(new TextItem(this, nullptr, text));
	is_menu = false;
}

void Textbox::SetMenu(bool menu, unsigned char display_count, sf::Vector2i start, sf::Vector2u spacing, std::function<void(TextItem* source)> callback, unsigned int flags, unsigned int scroll_start, std::function<void()> switch_callback, bool can_switch_last, sf::Vector2i arrow_off)
{
	this->is_menu = menu;
	this->display_count = display_count;
	this->item_start = start;
	this->item_spacing = spacing;
	this->arrow_offset = arrow_off;
	this->menu_flags = flags;
	this->scroll_start = scroll_start;
	this->switch_last_item = can_switch_last;
	//this->menu_open_delay = MENU_DELAY_TIME;

	//for assigning close_callback, you'd think if we passed nullptr to callback and assigned close_callback to callback, close_callback would be assigned nullptr
	//but no, for some stupid reason the gcc doesn't like that. >:(
	if (callback)
		this->close_callback = callback;
	else
		this->close_callback = nullptr;
	if (switch_callback)
		this->switch_callback = switch_callback;
	else
		this->switch_callback = nullptr;
	//this->close_callback = (callback ? nullptr : callback);
	if (this->delete_on_close)
	{
		for (unsigned int i = 0; i < items.size(); i++)
		{
			if (items[i])
				delete items[i];
		}
	}
	this->items.resize(0);
	UpdateMenu();
}

void Textbox::ClearItems()
{
	//if (delete_on_close)
	//{
	for (unsigned int i = 0; i < items.size(); i++)
	{
		if (items[i])
			delete items[i];
	}
	//}
	items.clear();
}

void Textbox::SetCounter(bool is_counter, unsigned char min, unsigned char max, std::function<void()> callback, std::function<void(TextItem* source)> close_callback)
{
	this->is_counter = is_counter;
	this->min_counter = min;
	this->max_counter = max;
	this->counter_value = min;
	if (callback)
		this->counter_callback = callback;
	else
		this->counter_callback = nullptr;
	if (close_callback)
		this->close_callback = close_callback;
	else
		this->close_callback = nullptr;

	UpdateCounter();
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
			if (!hide_frame || menu_open_delay != 0)
				sprite8x8.setTexture(*frame);
			else
				sprite8x8.setTexture(*ResourceCache::GetStatusesTexture(0));
			unsigned char tile = MENU_BLANK;
			if (x == pos.x && y == pos.y)
				tile = (hide_frame ? MENU_BLANK : MENU_CORNER_UL);
			else if (x == pos.x + size.x - 1 && y == pos.y)
				tile = (hide_frame ? MENU_BLANK : MENU_CORNER_UR);
			else if (!hide_frame && x == pos.x && y == pos.y + size.y - 1)
				tile = (hide_frame ? MENU_BLANK : MENU_CORNER_DL);
			else if (!hide_frame && x == pos.x + size.x - 1 && y == pos.y + size.y - 1)
				tile = (hide_frame ? MENU_BLANK : MENU_CORNER_DR);
			else if (y == pos.y || (!hide_frame && y == pos.y + size.y - 1))
				tile = (hide_frame ? MENU_BLANK : MENU_H);
			else if (x == pos.x || x == pos.x + size.x - 1)
				tile = (hide_frame ? MENU_BLANK : MENU_V);
			else
			{
				if (x == pos.x + size.x - 2 && y == pos.y + size.y - 2 && arrow_timer > CURSOR_MORE_TIME / 2)
				{
					tile = CURSOR_MORE;
					sprite8x8.setTexture(*ResourceCache::GetFontTexture());
				}
				else if (menu_open_delay == 0)
				{
					tile = (unsigned char)tiles[(x - pos.x - 1) + (y - pos.y - 1) * (size.x - 2)];
					if (tile >= 0x80) //use the font texture
						sprite8x8.setTexture(*ResourceCache::GetFontTexture());
					else if (tile == MENU_BLANK)
						sprite8x8.setTexture(*frame);
					/*else if (tile >= 0x10 && tile < 0x40)
					{
						sprite8x8.setTexture(*ResourceCache::GetStatusesTexture(0));
						tile -= 0x10;
					}*/
					tile &= 0x7F;
				}
				else
					tile = MENU_BLANK; //has intentionally making something seem like it's lagging ever been done before?
			}
			if (tile == MENU_BLANK)
				sprite8x8.setTexture(*frame);

			src_rect.left = (tile % 16) * 8;
			src_rect.top = (tile / 16) * 8;
			sprite8x8.setTextureRect(src_rect);
			sprite8x8.setPosition((float)(x * 8), (float)(y * 8));

			window->draw(sprite8x8);
		}
	}

	//This code to draw arrows must be here instead of in the Render function where it was before.
	//They get drawn on top of children otherwise.
	if (is_menu && (menu_flags & MenuFlags::FOCUSABLE) && menu_open_delay == 0)
	{
		if (arrow_state & ArrowStates::INACTIVE)
			DrawArrow(window, false);
		if (arrow_state & ArrowStates::ACTIVE && cursor_visibility_timer < CURSOR_VIS_TIME / 2)
			DrawArrow(window, true);
	}

	if (!menu_open_delay && render_callback != nullptr)
		render_callback(window);

	for (unsigned int i = 0; i < textboxes.size(); i++)
		textboxes[i]->Render(window);
}

void Textbox::UpdateMenu()
{
	memset(tiles, MENU_BLANK, (size.x - 2) * (size.y - 1));
	//this function creates "tiles" for display
	for (unsigned int i = scroll_pos; i < scroll_pos + display_count && i < items.size(); i++)
	{
		TextItem* item = items[i];
		if (!item)
			continue;
		unsigned int x = item_start.x;
		unsigned int y = item_start.y + (i - scroll_pos) * item_spacing.y;
		for (unsigned int c = 0; c < item->text.length() && x + y * (size.x - 2) < (size.x - 2) * (size.y - 1); c++)
		{
			unsigned char at = item->text[c];
			if (at == MESSAGE_LINE)
			{
				x = item_start.x;
				y++;
				continue;
			}
			tiles[x + y * (size.x - 2)] = item->text[c];
			x++;
		}
	}
}

void Textbox::UpdateCounter()
{
	string s = "*";
	s.append((counter_value < 10 ? "0" : ""));
	s.append(itos(counter_value));
	pokestring(s);
	for (unsigned int i = 0; i < s.length(); i++)
		tiles[i] = (unsigned char)s[i];
}

void Textbox::DrawArrow(sf::RenderWindow* window, bool active)
{
	sf::IntRect src_rect = sf::IntRect(0, 0, 8, 8);
	sprite8x8.setTexture(*ResourceCache::GetFontTexture());

	src_rect.left = ((active ? CURSOR_ACTIVE : CURSOR_INACTIVE) % 16) * 8;
	src_rect.top = ((active ? CURSOR_ACTIVE : CURSOR_INACTIVE) / 16) * 8;
	int index = (int)((active ? active_index : inactive_index) - scroll_pos);
	if (index < 0 || index >= display_count)
		return;
	sprite8x8.setPosition((float)(pos.x * 8 + item_start.x * 8 + arrow_offset.x * 8), (float)(pos.y * 8 + item_start.y * 8 + 8 + index * 8 * item_spacing.y + arrow_offset.y * 8));
	sprite8x8.setTextureRect(src_rect);
	window->draw(sprite8x8);
}

void Textbox::ProcessNextCharacter()
{
	//if (text_pos >= text->GetText().length())
	//	return;

	unsigned char c = text->GetText()[text_pos];
	switch (c)
	{
	case 0: //end
	case MESSAGE_END: //end
		if (InputController::KeyDownOnce(INPUT_A) || InputController::KeyDownOnce(INPUT_B))
		{
			Close();
			text->Action();
			if (textboxes.size() > 0)
				close_when_no_children = true;
		}
		return;

	case MESSAGE_LINE: //new line
		text_tile_pos = (text_tile_pos + ((size.x - 2) - text_tile_pos)) % ((size.x - 2) * (size.y - 2)) * 2 + size.x - 2;
		break;

	case MESSAGE_PARA: //require input to continue
		if (InputController::KeyDownOnce(INPUT_A) || InputController::KeyDownOnce(INPUT_B))
		{
			memset(tiles, MENU_BLANK, (size.x - 2) * (size.y - 2));
			text_tile_pos = size.x - 2;
			text_timer = TEXT_TIMER_BLANK;
			//text_speed = 3;
			break;
		}
		else if (arrow_timer == 0)
			arrow_timer = CURSOR_MORE_TIME;
		return;

	case MESSAGE_SCROLL: //scroll down one line
		if (InputController::KeyDownOnce(INPUT_A) || InputController::KeyDownOnce(INPUT_B))
		{
			scroll_timer = 10;
			arrow_timer = 0;
			text_timer = 0;
			text_pos++;
		}
		else if (arrow_timer == 0)
			arrow_timer = CURSOR_MORE_TIME;
		return;

	case MESSAGE_PROMPT: //prompt for continue
		if (InputController::KeyDownOnce(INPUT_A) || InputController::KeyDownOnce(INPUT_B))
		{
			Close();
			text->Action();
			if (textboxes.size() > 0)
				close_when_no_children = true;
			return;
		}
		else if (arrow_timer == 0)
			arrow_timer = CURSOR_MORE_TIME;
		return;

	case MESSAGE_AUTOCLOSE: //autoclose the message
		auto_close_timer = 1;
		//text_pos--;
		break;

	default: //regular char
		tiles[text_tile_pos++] = c;
		break;
	}

	//according to padz, the game prints out one character every frame
	//which means at 60fps with a full-screen update, the text looks too smooth.
	//however, the game updates the screen in thirds, which means 3 characters/
	//will appear every three frames. that's what we must do.
	if (InputController::KeyDownOnce(INPUT_A) || InputController::KeyDownOnce(INPUT_B))
	{
		if (text_timer == 0)
			text_timer = TEXT_TIMER_FAST;
		text_speed = TEXT_SPEED_FAST;
	}
	else if (!InputController::KeyDown(INPUT_A) && !InputController::KeyDown(INPUT_B))
	{
		text_speed = Players::GetPlayer1()->GetOptions().text_speed;
		if (text_timer == 0)
		{
			if (text_speed == TEXT_SPEED_SLOW)
				text_timer = TEXT_TIMER_SLOW;
			else
				text_timer = TEXT_TIMER_FAST;
		}
	}
	else
	{
		if (text_timer == 0)
			text_timer = TEXT_TIMER_FAST;
		text_speed = TEXT_SPEED_FAST;
	}

	arrow_timer = 0;
	//text_timer = text_speed;
	text_pos++;
}

void Textbox::Close(bool ignore_callback)
{
	close = true;
	if (!ignore_callback && this->close_callback != nullptr)
		close_callback(0);
	if (close && (arrow_state & ArrowStates::ACTIVE) && (arrow_state & ArrowStates::INACTIVE))
	{
		inactive_index = active_index;
		SetArrowState(ArrowStates::ACTIVE);
	}
}

void Textbox::Scroll(bool up)
{
	if (up)
	{
		if (active_index > 0)
		{
			active_index--;
			if (GetScrollIndex() < 0)
			{
				scroll_pos--;
				if (scroll_pos + display_count < items.size())
					arrow_timer = CURSOR_MORE_TIME;
				else
					arrow_timer = 0;
				UpdateMenu();
			}
		}
		else if (menu_flags & MenuFlags::WRAPS)
		{
			active_index = items.size() - 1;
		}
	}
	else
	{
		if (active_index < items.size() - 1)
		{
			active_index++;
			if (GetScrollIndex() >(int)scroll_start)
			{
				scroll_pos++;
				if (scroll_pos + display_count < items.size())
					arrow_timer = CURSOR_MORE_TIME;
				else
					arrow_timer = 0;
				UpdateMenu();
			}
		}
		else if (menu_flags & MenuFlags::WRAPS)
		{
			active_index = 0;
		}
	}
}

void Textbox::InitializeSwitch()
{
	SetArrowState(ArrowStates::ACTIVE | ArrowStates::INACTIVE);
	inactive_index = active_index;
}