#pragma once

#include "ResourceCache.h"

struct Move
{
	unsigned char index;
	unsigned char animation;
	unsigned char effect;
	unsigned char power;
	unsigned char type;
	unsigned char accuracy;
	unsigned char max_pp;
	unsigned char original_max_pp;
	unsigned char pp;
	unsigned char pp_ups;
	string name;

	Move() : Move(0)
	{

	}

	Move(unsigned char id)
	{
		//the game keeps track of pp ups by using the last 2 bits in the max pp
		//however we don't have strict memory limitations and that's more work that i have to do
		//so we're going to do this the easy way.
		pp_ups = 0;
		index = id;
		memcpy(&animation, ResourceCache::GetMoveData()->data_start + 6 * id - 6, 6);
		original_max_pp = max_pp;
		pp = rand() % max_pp + 1;
		//woooaaaahhhhhhh like it's cool to be able to do this. i feel naughty.
		if (id > 0)
			name = ResourceCache::GetMoveName(id - 1);
		else
			name = pokestring("-");
	}
};