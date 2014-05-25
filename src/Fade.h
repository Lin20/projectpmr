#pragma once

#include <string.h>
#include <SFML/Graphics.hpp>
#include "Events.h"
/* This whole class is a mess.
 * I don't like how it's an entire class definition, but none of the functions are
 * long/complicated enough (in my opinion) to create a regular cpp file containing the
 * function definitions.
 * Not to mention the way I wrote this class is ugly... but it works.
 */

class Fade
{
public:
	Fade(unsigned int start = 0)
	{
		this->fade_timer = 0;
		this->start_fade = start;
		this->warp_to.dest_map = 254;
	}
	~Fade() { }

	void Start(Warp warp)
	{
		warp_to = warp;
		start_fade = 30;
		fade_timer = 1;
		previous_fade_index = 0xFF;
		done = false;
	}

	void Reset()
	{
		fade_timer = 0;
		start_fade = 0;
		previous_fade_index = 0;
		done = true;
	}

	void Update()
	{
		previous_fade_index = CurrentFade();
		if (fade_timer < start_fade)
			fade_timer++;
		else
			Reset();
	}

	void SetFadeToBlack(sf::Color base[4])
	{
		for (int i = 0; i < 4; i++)
			memcpy(palettes[i], base, sizeof(sf::Color) * 4);
		palettes[1][0] = base[1];
		palettes[2][0] = base[2];
		palettes[2][1] = base[3];
		for (int i = 0; i < 3; i++)
			palettes[3][i] = base[3];
	}

	unsigned int CurrentFade() { return min(3u, (fade_timer > 0 ? fade_timer / (start_fade / 4) + 1 : 1)); }
	unsigned int LastFade() { return previous_fade_index; }
	sf::Color* GetCurrentPalette() { return palettes[CurrentFade()]; }

	Warp GetWarpTo() { return warp_to; }
	void SetWarpTo(Warp w) { warp_to = w; }

	bool Done() { return done; }


private:
	sf::Color palettes[4][4]; //[palette index][color index]
	unsigned int fade_timer;
	unsigned int start_fade;
	unsigned int previous_fade_index;
	bool done;
	Warp warp_to; //warp
};
