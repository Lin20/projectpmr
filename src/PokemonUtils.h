#pragma once

#include "Pokemon.h"
#include "Textbox.h"
#include "EvolutionScreen.h"

//this is a class for handling menu-related pokemon actions, like displaying stats and leveling up.
//otherwise there would have to be code to display stats in 3 different places
//the summary, menu level-up, and battle level-up. i don't like that.
class PokemonUtils
{
public:
	static void WriteStats(Textbox* t, Pokemon* p, char x);
	static void Levelup(TextItem* t, Pokemon* p, std::function<void(TextItem*)> action_callback);
	static std::function<void(TextItem* s)> LearnMove(Textbox* src, Pokemon* p, unsigned char move, bool close_src = true);
	static std::function<void(TextItem* s)> Evolve(Textbox* src, Pokemon* p, unsigned char evolution, bool close_src = true);
	static std::function<void(TextItem* s)> CheckMove(Textbox* src, Pokemon* p);
};

