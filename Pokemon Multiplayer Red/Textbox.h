#pragma once

#include <vector>
#include <SFML\Graphics.hpp>
#include "Common.h"
#include "ResourceCache.h"
#include "TextItem.h"
#include "StringConverter.h"

class Textbox
{
public:
	Textbox(unsigned char x = 0, unsigned char y = 0, unsigned char width = 0, unsigned char height = 0);
	~Textbox();

	void Update();
	void Render(sf::RenderWindow* window);

	void SetFrame(unsigned char x, unsigned char y, unsigned char width, unsigned char height);
	void SetMenu(bool menu, unsigned char display_count, sf::Vector2u start, sf::Vector2u spacing, bool delete_items = true);
	
	void UpdateMenu();

	sf::Vector2u GetPosition() { return pos; }
	sf::Vector2u GetSize() { return size; }
	vector<TextItem*>& GetItems() { return items; }

private:
	sf::Vector2u pos;
	sf::Vector2u size;

	//menu-related stuff
	bool is_menu; //is this textbox a menu?
	unsigned char display_count; //how many items get displayed?
	unsigned char scroll_start; //when does the scrolling start? eg. 3/4 in the inventory
	vector<TextItem*> items; //the items get displayed
	bool delete_items;

	//text-box related stuff
	string text; //the text that is going to be displayed (null-terminated)
	unsigned int text_pos; //the next char to parse
	unsigned char* tiles; //the parsed text so several characters don't have to be parsed each frame
	bool autoscroll; //does the text scroll if the player doesn't hit a button? (eg. in battles)
	int text_timer; //the time until the next char is parsed
	sf::Vector2u item_start; //where the items start drawing (relative to pos)
	sf::Vector2u item_spacing; //the space between the items

	//render stuff
	sf::Sprite sprite8x8;
	void DrawFrame(sf::RenderWindow* window);
	void DrawMenu(sf::RenderWindow* window);
};