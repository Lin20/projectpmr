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
	unsigned char pp;
	string name;

	Move() : Move(0)
	{

	}

	Move(unsigned char id)
	{
		index = id;
		memcpy(&animation, ResourceCache::GetMoveData()->data_start + 6 * id - 6, 6);
		pp = max_pp;
		//woooaaaahhhhhhh like it's cool to be able to do this. i feel naughty.
		if (id > 0)
			name = ResourceCache::GetMoveName(id - 1);
		else
			name = pokestring("-");
	}
};