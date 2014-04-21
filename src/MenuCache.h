#pragma once

#include "Textbox.h"
#include "Players.h"

class MenuCache
{
public:
	MenuCache();
	~MenuCache();

	static void ReleaseResources();
	static Textbox* StartMenu(std::vector<Textbox*>* owner = 0);
	static Textbox* DebugMenu(std::vector<Textbox*>* owner = 0);

private:
	static Textbox* start_menu;
	static Textbox* debug_menu;
};