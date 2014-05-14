#pragma once

#include "Pokemon.h"
#include "Textbox.h"

//this is a class for handling menu-related pokemon actions, like displaying stats and leveling up.
//otherwise there would have to be code to display stats in 3 different places
//the summary, menu level-up, and battle level-up. i don't like that.
class PokemonUtils
{
public:
	static void WriteStats(Textbox* t, Pokemon* p, char x);
};

