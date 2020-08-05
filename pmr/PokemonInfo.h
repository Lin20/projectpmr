#pragma once

#include "Textbox.h"
#include "Pokemon.h"
#include "TileMap.h"
#include "PokemonUtils.h"

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
	void UpdatePokemon(Pokemon** party, bool show_able_notable = false);
	void UpdateOnePokemon(unsigned char index, bool is_able = false);
	void DrawHPBars(sf::RenderWindow* window);
	void DrawIcons(sf::RenderWindow* window);
	Pokemon** GetParty() { return party; }
	unsigned char CalculateHPBars(unsigned int hp, unsigned int max_hp);
	void SwapPokemon();
	void DisplaySummary(Pokemon* p);
	void DisplaySummary2(Pokemon* p);

	void DrawHPBar(sf::RenderWindow* window, sf::Sprite& sprite8x8, sf::IntRect& src_rect, int x, int y, Pokemon* p, unsigned int pixels);

	void Heal(int amount, std::function<void(TextItem* src)> finished);
	int GetHPAmountChanged() { return hp_amount_changed; }

	void SetAbleNotAble(bool* values) {
		memcpy(ability, values, sizeof(bool)* 6);
		ability[0] = values[0];
	}
	bool* GetAblility() { return ability; }

private:
	Textbox* menu;
	Textbox* choose_textbox; //the textbox that appears at the bottom displaying "Choose a POKEMON."
	TextboxParent* parent;
	Pokemon** party;
	unsigned char selection_delay;
	unsigned char last_hover;
	bool ability[6];
	bool show_able_notable;

	//hp recovery and draining
	int delta_hp;
	int hp_amount_changed;
	unsigned char delta_hp_timer;
	unsigned int selected_bar_length;
	std::function<void(TextItem* src)> heal_callback;
};