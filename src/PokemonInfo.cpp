#include "PokemonInfo.h"

PokemonInfo::PokemonInfo()
{
	menu = 0;
	choose_textbox = 0;
	selection_delay = 0;

	//although the bottom textbox looks like an ordinary textbox,
	//the current textbox class doesn't support instant full printout of text.
	//so we can just fake it by making it a display-only menu
	choose_textbox = new Textbox(0, 12, 20, 6, false);
	choose_textbox->SetMenu(true, 1, sf::Vector2i(0, 1), sf::Vector2u(0, 2));
	choose_textbox->GetItems().push_back(new TextItem(choose_textbox, nullptr, pokestring("Choose a #MON.")));
	choose_textbox->UpdateMenu();

	//i spent over an hour on this trying to find out why it wouldn't switch properly.
	//see that commented out UpdatePokemon call? yeah. turns out the items get switched
	//automatically after the callback. i'm such a sly devil.
	//sigh.
	auto swapped = [this]()
	{
		Pokemon* from = this->GetParty()[this->GetMenu()->GetInactiveIndex()];
		this->GetParty()[this->GetMenu()->GetInactiveIndex()] = this->GetParty()[this->GetMenu()->GetActiveIndex()];
		this->GetParty()[this->GetMenu()->GetActiveIndex()] = from;
		//this->UpdatePokemon(this->GetParty());
	};

	menu = new Textbox(-1, -1, 22, 13, false, true);
	menu->SetMenu(true, 6, sf::Vector2i(3, 0), sf::Vector2u(0, 2), [this](TextItem* src)
	{
		this->GetChooseTextbox()->Close();
	}, MenuFlags::FOCUSABLE | MenuFlags::SWITCHABLE | MenuFlags::WRAPS, INT_MAX, swapped, true, sf::Vector2i(-2, 1));
	menu->SetArrowState(ArrowStates::ACTIVE);
	menu->UpdateMenu();
	menu->SetRenderCallback([this](sf::RenderWindow* w) { this->DrawIcons(w); });
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

void PokemonInfo::UpdatePokemon(Pokemon** party)
{
	this->party = party;
	menu->ClearItems();
	for (int i = 0; i < 6; i++)
	{
		string s = party[i]->nickname;
		while (s.length() < 10)
			s.insert(s.end(), MENU_BLANK);
		//now append the level
		if (party[i]->level < 100)
			s.insert(s.end(), 0x30); //L:
		s.append(pokestring(itos(party[i]->level)));
		s.insert(s.end(), MESSAGE_LINE);
		s.insert(s.end(), MENU_BLANK);
		s.insert(s.end(), 0x33); //H
		s.insert(s.end(), 0x20); //P:

		unsigned char pixels = CalculateHPBars(party[i]->hp, party[i]->max_hp);
		for (int h = 0; h < 6; h++)
		{
			unsigned char c = 0x29; //full
			if (pixels == 0)
				c = 0x21;
			else if (pixels < 8)
			{
				c = 0x20 + pixels;
				pixels = 0;
			}
			else
				pixels -= 8;
			
			s.insert(s.end(), c);
		}
		s.insert(s.end(), 0x2A);
		if (party[i]->hp < 10)
			s.insert(s.end(), MENU_BLANK);
		if (party[i]->hp < 100)
			s.insert(s.end(), MENU_BLANK);
		s.append(pokestring(itos(party[i]->hp).append("/")));

		if (party[i]->max_hp < 10)
			s.insert(s.end(), MENU_BLANK);
		if (party[i]->max_hp < 100)
			s.insert(s.end(), MENU_BLANK);
		s.append(pokestring(itos(party[i]->max_hp)));

		menu->GetItems().push_back(new TextItem(menu, nullptr, s, i, party[i]->id));
	}
	menu->UpdateMenu();
}

void PokemonInfo::DrawIcons(sf::RenderWindow* window)
{
	selection_delay++;
	//if (selection_delay >=)
	sf::IntRect src_rect = sf::IntRect(0, 0, 8, 8);
	sf::Sprite sprite8x8;
	sprite8x8.setTexture(*ResourceCache::GetPokemonIcons());
	int dest_x, dest_y;
	bool mirrored = false;
	for (int i = 0; i < 6; i++)
	{
		unsigned char c = ResourceCache::GetIconIndex(party[i]->pokedex_index - 1);
		if (c == 0 || c == 3 || c >= 6)
			mirrored = true;
		else
			mirrored = false;
		for (unsigned int y = 0; y < 2; y++)
		{
			for (unsigned int x = 0; x < 2; x++)
			{
				unsigned char t = c * 4 + y * 2 + (!mirrored && x == 1 ? x : 0);
				src_rect.left = (t % 16) * 8 + (mirrored && x == 1 ? 8 : 0);
				src_rect.top = (t / 16) * 8;
				src_rect.width = (mirrored && x == 1 ? -8 : 8);
				sprite8x8.setTextureRect(src_rect);

				dest_x = (int)(8 + x * 8 + (mirrored && x == 1 ? 1 - x : 0) * 8);
				dest_y = (int)(i * 16 + y * 8);
				sprite8x8.setPosition(dest_x, dest_y);
				window->draw(sprite8x8);
			}
		}
	}
}

unsigned char PokemonInfo::CalculateHPBars(unsigned int hp, unsigned int max_hp)
{
	if (hp == 0)
		return 0;
	if (max_hp < 256)
		return max(1u, hp * 48 / max_hp);
	return max(1u, ((hp * 48) >> 2) / (max_hp >> 2)); //interesting how the game does this.
	//very simple, but it took waaaaay too long to figure out.
}