#include "MenuCache.h"

Textbox* MenuCache::start_menu = 0;
Textbox* MenuCache::debug_menu = 0;

MenuCache::MenuCache()
{
}

MenuCache::~MenuCache()
{
}

void MenuCache::ReleaseResources()
{
	if (start_menu)
		delete start_menu;
	if (debug_menu)
		delete debug_menu;
}

//Return the start menu or create it if it doesn't exist
//Since the game starts without the pokedex, we shouldn't add it here but in MapScene
Textbox* MenuCache::StartMenu()
{
	if (start_menu)
		return start_menu;

	start_menu = new Textbox(10, 0, 10, 16);
	start_menu->SetMenu(true, 7, sf::Vector2i(1, 1), sf::Vector2u(0, 2), MenuFlags::FOCUSABLE | MenuFlags::WRAPS);
	start_menu->GetItems().push_back(new TextItem(start_menu, "POKéDEX", 0));
	start_menu->GetItems().push_back(new TextItem(start_menu, "POKéMON", 1));
	start_menu->GetItems().push_back(new TextItem(start_menu, "ITEMS", 2));
	start_menu->GetItems().push_back(new TextItem(start_menu, "Lin", 3));
	start_menu->GetItems().push_back(new TextItem(start_menu, "SAVE", 4));
	start_menu->GetItems().push_back(new TextItem(start_menu, "OPTIONS", 5));
	start_menu->GetItems().push_back(new TextItem(start_menu, "EXIT", 6));

	start_menu->UpdateMenu();
	return start_menu;
}

Textbox* MenuCache::DebugMenu()
{
	if (debug_menu)
		return debug_menu;

	debug_menu = new Textbox(0, 8, 14, 7);
	debug_menu->SetMenu(true, 3, sf::Vector2i(1, 1), sf::Vector2u(0, 1), MenuFlags::FOCUSABLE);
	debug_menu->GetItems().push_back(new TextItem(debug_menu, "Testing"));
	debug_menu->GetItems().push_back(new TextItem(debug_menu, "multiple"));
	debug_menu->GetItems().push_back(new TextItem(debug_menu, "menus."));

	debug_menu->UpdateMenu();
	return debug_menu;
}