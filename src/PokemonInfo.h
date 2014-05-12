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
	void Show(Textbox* parent, string text, std::function<void(TextItem* src)> select_callback, std::function<void(TextItem* src)> close_callback);
	Textbox* GetChooseTextbox() { return choose_textbox; }
	Textbox* GetMenu() { return menu; }
	void FocusChooseTextbox();
	void UpdatePokemon(Pokemon** party);
	void DrawHPBars(sf::RenderWindow* window);
	void DrawIcons(sf::RenderWindow* window);
	Pokemon** GetParty() { return party; }
	unsigned char CalculateHPBars(unsigned int hp, unsigned int max_hp);
	void SwapPokemon();
	void DisplaySummary(Pokemon* p);
	void DisplaySummary2(Pokemon* p);

	void DrawHPBar(sf::RenderWindow* window, sf::Sprite& sprite8x8, sf::IntRect& src_rect, int x, int y, Pokemon* p, unsigned int pixels);

	void Heal(int amount);

private:
	Textbox* menu;
	Textbox* choose_textbox; //the textbox that appears at the bottom displaying "Choose a POKEMON."
	TextboxParent* parent;
	Pokemon** party;
	unsigned char selection_delay;
	unsigned char last_hover;

	//hp recovery and draining
	int delta_hp;
	unsigned char delta_hp_timer;
	unsigned int selected_bar_length;
};