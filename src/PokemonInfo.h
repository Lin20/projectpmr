#pragma once

#include "Textbox.h"
#include "Pokemon.h"

class PokemonInfo
{
public:
	PokemonInfo();
	~PokemonInfo();

	void Show(Textbox* parent);
	Textbox* GetMenu() { return menu; }
	void UpdatePokemon(Pokemon* party);

private:
	Textbox* menu;
	Textbox* choose_textbox; //the textbox that appears at the bottom displaying "Choose a POKEMON."
};