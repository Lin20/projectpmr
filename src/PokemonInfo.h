#pragma once

#include "Textbox.h"
#include "Pokemon.h"
#include "TileMap.h"

class PokemonInfo
{
public:
	PokemonInfo();
	~PokemonInfo();

	void Show(Textbox* parent);
	Textbox* GetChooseTextbox() { return choose_textbox; }
	Textbox* GetMenu() { return menu; }
	void UpdatePokemon(Pokemon** party);
	void DrawHPBars(sf::RenderWindow* window);
	void DrawIcons(sf::RenderWindow* window);
	Pokemon** GetParty() { return party; }
	unsigned char CalculateHPBars(unsigned int hp, unsigned int max_hp);
	void SwapPokemon();
	void DisplaySummary(Pokemon* p);

	void DrawHPBar(sf::RenderWindow* window, sf::Sprite& sprite8x8, sf::IntRect& src_rect, int x, int y, Pokemon* p);

private:
	Textbox* menu;
	Textbox* choose_textbox; //the textbox that appears at the bottom displaying "Choose a POKEMON."
	Pokemon** party;
	unsigned char selection_delay;
	unsigned char last_hover;
};