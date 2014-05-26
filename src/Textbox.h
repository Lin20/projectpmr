#pragma once

#include <cstring>
#include <climits>
#include <string>
#include "Common.h"
#include "TextboxParent.h"
#include <vector>
#include <SFML/Graphics.hpp>
#include "ResourceCache.h"
#include "TextItem.h"
#include "StringConverter.h"
#include "InputController.h"
#include "Players.h"

class Textbox : public TextboxParent
{
public:
	Textbox(char x = 0, char y = 12, unsigned char width = 20, unsigned char height = 6, bool delete_on_close = true, bool hidden_frame = false);
	~Textbox();

	void Update();
	void Render(sf::RenderWindow* window);

	void SetFrame(char x, char y, unsigned char width, unsigned char height);
	void SetPosition(char x, char y);
	void SetText(TextItem* text);
	void SetText(string text);
	void SetMenu(bool menu, unsigned char display_count, sf::Vector2i start, sf::Vector2u spacing, std::function<void(TextItem* source)> = nullptr, unsigned int flags = MenuFlags::NONE, unsigned int scroll_start = INT_MAX, std::function<void()> switch_callback = nullptr, bool can_switch_last = true, sf::Vector2i arrow_offset = sf::Vector2i());
	void ClearItems();
	void SetCounter(bool is_counter, unsigned char min, unsigned char max, std::function<void()> action_callback = nullptr, std::function<void(TextItem* source)> close_callback = nullptr);

	void SetMenuFlags(unsigned int f) { menu_flags = f; }
	void SetArrowState(unsigned int f) { arrow_state = f; if (f == ArrowStates::INACTIVE) inactive_index = active_index; }
	void FlashCursor() { if (cursor_visibility_timer == 0) cursor_visibility_timer = CURSOR_VIS_TIME; }
	void SetCloseCallback(std::function<void(TextItem* source)> close_callback = nullptr)
	{
		if (close_callback)
			this->close_callback = close_callback;
		else
			this->close_callback = nullptr;
	}
	void SetTextTimer() { text_timer = 1; }

	void UpdateMenu();
	void UpdateCounter();
	void InitializeSwitch();

	sf::Vector2i GetPosition() { return pos; }
	sf::Vector2u GetSize() { return size; }
	vector<TextItem*>& GetItems() { return items; }
	unsigned int GetActiveIndex() { return active_index; }
	unsigned int GetInactiveIndex() { return inactive_index; }
	unsigned char GetCounterValue() { return counter_value; }
	unsigned char GetArrowState() { return arrow_state; }
	void ResetSelection() { active_index = inactive_index = scroll_pos = 0; }
	bool IsDone() { return auto_close_timer > 0; }
	void SetJustOpened(unsigned char delay = MENU_DELAY_TIME) { menu_open_delay = delay; }
	unsigned char* GetTiles() { return tiles; } //allows for external tile editing
	TextItem* GetText() { return text; }

	int GetScrollIndex() { return active_index - scroll_pos; }
	bool SetToClose() { return close; }
	void Close(bool ignore_callback = false);
	void CancelClose() { close = false; }
	bool DeleteOnClose() { return delete_on_close; }
	void CancelSwitch() { cancel_switch = true; }
	bool IsSwitchCanceled() { return cancel_switch; }
	void Reset() { text_pos = 0; }
	void PerformCloseCallback() { if (close_callback != nullptr)close_callback(0); }
	void SetMaxSelect(unsigned int value) { max_select = value; }

	void SetRenderCallback(std::function<void(sf::RenderWindow* w)> f)
	{
		if (f)
			this->render_callback = f;
		else
			this->render_callback = nullptr;
	}

private:
	sf::Vector2i pos;
	sf::Vector2u size;
	bool close;
	bool delete_on_close;
	bool cancel_switch;
	bool close_when_no_children; //make the textbox close when its children have been closed
	bool hide_frame;
	std::function<void(sf::RenderWindow* t)> render_callback; //function called when update loop finishes

	//menu-related stuff
	bool is_menu; //is this textbox a menu?
	unsigned char display_count; //how many items get displayed?
	unsigned int scroll_pos; //where the scroll is positioned
	unsigned int scroll_start; //when does the scrolling start? eg. 3/4 in the inventory
	vector<TextItem*> items; //the items get displayed
	sf::Vector2i item_start; //where the items start drawing (relative to pos)
	sf::Vector2u item_spacing; //the space between the items
	sf::Vector2i arrow_offset; //offset for the selection arrow (relative to default pos)
	std::function<void(TextItem* source)> close_callback; //the function that's called when the menu is closed; source will never be used but is there to match TextItem callbacks
	unsigned char menu_open_delay; //used to emulate lag when opening the menu
	unsigned int max_select; //used to determine how far down the cursor can go

	//menu selection stuff
	unsigned int active_index; //where the active arrow is
	unsigned int inactive_index; //where the inactive arrow is
	unsigned int menu_flags; //uses the MenuFlags enum
	unsigned int arrow_state; //uses the ArrowStates enum
	unsigned char cursor_visibility_timer; //timer for hiding and showing the selection cursor
	std::function<void()> switch_callback; //function that gets called when item is swapped
	bool switch_last_item;

	//text-box related stuff
	TextItem* text; //the text that is going to be displayed (null-terminated)
	unsigned int text_tile_pos; //the tile index for the text character
	unsigned int text_pos; //the next char to parse
	unsigned char* tiles; //the parsed text so several characters don't have to be parsed each frame
	bool autoscroll; //does the text scroll if the player doesn't hit a button? (eg. in battles)
	int text_timer; //the time until the next char is parsed
	unsigned char arrow_timer; //if > CURSOR_NEXT_TIME / 2 then show "more" arrow
	unsigned char text_speed; //speed the text moves at
	unsigned char scroll_timer; //stage of text scrolling
	unsigned char auto_close_timer; //if > 0, close when it hits (x)

	//counter stuff
	bool is_counter; //is the textbox a counter?
	unsigned char min_counter;
	unsigned char max_counter;
	unsigned char counter_value;
	std::function<void()> counter_callback; //the function that's called when confirmed
	std::function<void()> counter_close_callback; //the function that's called when confirmed

	//render stuff
	sf::Sprite sprite8x8;
	void DrawFrame(sf::RenderWindow* window);
	void DrawArrow(sf::RenderWindow* window, bool active);
	void ProcessNextCharacter();

	void Scroll(bool up);
};
