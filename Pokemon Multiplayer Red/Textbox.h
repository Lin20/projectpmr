#pragma once

#include <vector>
#include <SFML\Graphics.hpp>
#include "Common.h"
#include "ResourceCache.h"
#include "TextItem.h"
#include "StringConverter.h"
#include "InputController.h"

class Textbox
{
public:
	Textbox(unsigned char x = 0, unsigned char y = 0, unsigned char width = 0, unsigned char height = 0);
	~Textbox();

	void Update();
	void Render(sf::RenderWindow* window);

	void SetFrame(unsigned char x, unsigned char y, unsigned char width, unsigned char height);
	void SetMenu(bool menu, unsigned char display_count, sf::Vector2i start, sf::Vector2u spacing, unsigned int flags = MenuFlags::NONE, bool delete_items = true);

	void SetMenuFlags(unsigned int f) { menu_flags = f; }
	void SetArrowState(unsigned int f) { arrow_state = f; }
	
	void UpdateMenu();

	sf::Vector2i GetPosition() { return pos; }
	sf::Vector2u GetSize() { return size; }
	vector<TextItem*>& GetItems() { return items; }

	int GetScrollIndex() { return active_index - scroll_start; }

private:
	sf::Vector2i pos;
	sf::Vector2u size;

	//menu-related stuff
	bool is_menu; //is this textbox a menu?
	unsigned char display_count; //how many items get displayed?
	unsigned int scroll_start; //when does the scrolling start? eg. 3/4 in the inventory
	vector<TextItem*> items; //the items get displayed
	bool delete_items;
	sf::Vector2i item_start; //where the items start drawing (relative to pos)
	sf::Vector2u item_spacing; //the space between the items

	//menu selection stuff
	unsigned int active_index; //where the active arrow is
	unsigned int inactive_index; //where the inactive arrow is
	unsigned int menu_flags; //uses the MenuFlags enum
	unsigned int arrow_state; //uses the ArrowStates enum

	//text-box related stuff
	string text; //the text that is going to be displayed (null-terminated)
	unsigned int text_pos; //the next char to parse
	unsigned char* tiles; //the parsed text so several characters don't have to be parsed each frame
	bool autoscroll; //does the text scroll if the player doesn't hit a button? (eg. in battles)
	int text_timer; //the time until the next char is parsed

	//render stuff
	sf::Sprite sprite8x8;
	void DrawFrame(sf::RenderWindow* window);
	void DrawArrow(sf::RenderWindow* window, bool active);
};