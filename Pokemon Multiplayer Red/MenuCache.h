#pragma once

#include "Textbox.h"

class MenuCache
{
public:
	MenuCache();
	~MenuCache();

	static Textbox* StartMenu();
	static Textbox* DebugMenu();

private:
	static Textbox* start_menu;
	static Textbox* debug_menu;
};