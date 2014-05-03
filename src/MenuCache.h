#pragma once

#include "Textbox.h"
#include "Players.h"
#include "PokemonInfo.h"

class MenuCache
{
public:
	MenuCache();
	~MenuCache();

	static void ReleaseResources();
	static Textbox* StartMenu(std::vector<Textbox*>* owner = 0);
	static Textbox* DebugMenu(std::vector<Textbox*>* owner = 0);
	static PokemonInfo* PokemonMenu();

private:
	static Textbox* start_menu;
	static Textbox* debug_menu;
	static PokemonInfo* pokemon_menu;
};