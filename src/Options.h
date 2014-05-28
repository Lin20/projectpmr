#pragma once

#include "Constants.h"

struct Options
{
	unsigned char text_speed;
	unsigned char player_sprite;

	Options()
	{
		text_speed = TEXT_SPEED_FAST;
		player_sprite = 1;
	}
};

