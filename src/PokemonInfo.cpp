#include "PokemonInfo.h"

PokemonInfo::PokemonInfo()
{
	menu = 0;
	choose_textbox = 0;

	//although the bottom textbox looks like an ordinary textbox,
	//the current textbox class doesn't support instant full printout of text.
	//so we can just fake it by making it a display-only menu
	choose_textbox = new Textbox();
	choose_textbox->SetMenu(true, 1, sf::Vector2i(0, 1), sf::Vector2u(0, 2));
	choose_textbox->GetItems().push_back(new TextItem(choose_textbox, nullptr, pokestring("Choose a #MON.")));
	choose_textbox->UpdateMenu();

	menu = new Textbox(-1, -1, 22, 13, false, true);
	menu->SetMenu(true, 6, sf::Vector2i(3, 0), sf::Vector2u(0, 2), nullptr, MenuFlags::FOCUSABLE | MenuFlags::SWITCHABLE | MenuFlags::WRAPS, INT_MAX, nullptr, true, sf::Vector2i(-2, 1));
	menu->SetArrowState(ArrowStates::ACTIVE);
	menu->UpdateMenu();
}

PokemonInfo::~PokemonInfo()
{
	if (menu)
		delete menu;
	if (choose_textbox)
		delete choose_textbox;
}

void PokemonInfo::Show(Textbox* parent)
{
	parent->ShowTextbox(choose_textbox);
	parent->ShowTextbox(menu);
}

void PokemonInfo::UpdatePokemon(Pokemon* party)
{
	menu->ClearItems();
	for (int i = 0; i < 6; i++)
	{
		menu->GetItems().push_back(new TextItem(menu, nullptr, party[i].nickname, i, party[i].id));
	}
	menu->UpdateMenu();
}