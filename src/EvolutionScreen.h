#pragma once

#include "Pokemon.h"
#include "Textbox.h"
#include "ResourceCache.h"

class EvolutionScreen
{
public:
	EvolutionScreen(Pokemon* p, unsigned char evolution);
	~EvolutionScreen();

	void Show(Textbox* src);
	void Render(sf::RenderWindow* window);

private:
	Textbox* parent;
	Textbox* main_frame;
	Pokemon* pokemon;
	Pokemon* pokemon_to;

	PaletteTexture* from_black;
	PaletteTexture* to_black;

	unsigned char begin_timer;
	unsigned char frames;
	unsigned char delay;
	unsigned char color_timer;
	unsigned char frames_left;
	unsigned char delay_left;

	void Finalize();
	void HitB();
};
