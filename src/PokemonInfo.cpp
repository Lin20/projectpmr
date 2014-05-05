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
		this->SwapPokemon();
		//this->UpdatePokemon(this->GetParty());
	};

	menu = new Textbox(-1, -1, 22, 13, false, true);
	menu->SetMenu(true, 6, sf::Vector2i(3, 0), sf::Vector2u(0, 2), [this](TextItem* src)
	{
		this->GetChooseTextbox()->Close();
	}, MenuFlags::FOCUSABLE | MenuFlags::SWITCHABLE | MenuFlags::WRAPS | MenuFlags::A_TO_SWITCH, INT_MAX, swapped, true, sf::Vector2i(-2, 1));
	menu->SetArrowState(ArrowStates::ACTIVE);
	menu->UpdateMenu();
	menu->SetRenderCallback([this](sf::RenderWindow* w) { this->DrawHPBars(w); this->DrawIcons(w); });
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
	auto select = [this](TextItem* src)
	{
		this->GetMenu()->SetArrowState(ArrowStates::INACTIVE);
		Textbox* select = new Textbox(11, 11, 9, 7);
		select->SetMenu(true, 3, sf::Vector2i(1, 0), sf::Vector2u(0, 2), [this](TextItem* s) { this->GetMenu()->FlashCursor(); if (this->GetMenu()->GetArrowState() == ArrowStates::INACTIVE) this->GetMenu()->SetArrowState(ArrowStates::ACTIVE); }, MenuFlags::FOCUSABLE | MenuFlags::WRAPS);

		auto _switch = [this](TextItem* src)
		{
			this->GetChooseTextbox()->ClearItems();
			this->GetChooseTextbox()->GetItems().push_back(new TextItem(choose_textbox, nullptr, pokestring("Move #MON\n\nwhere?")));
			this->GetChooseTextbox()->UpdateMenu();
			this->GetMenu()->InitializeSwitch();
			src->GetParent()->Close();
		};

		select->SetArrowState(ArrowStates::ACTIVE);
		select->GetItems().push_back(new TextItem(select, [this](TextItem* src) { DisplaySummary(this->GetParty()[this->GetMenu()->GetActiveIndex()]); }, pokestring("STATS"), 0));
		select->GetItems().push_back(new TextItem(select, _switch, pokestring("SWITCH"), 0));
		select->GetItems().push_back(new TextItem(select, [this](TextItem* src) {this->GetMenu()->FlashCursor(); this->GetMenu()->SetArrowState(ArrowStates::ACTIVE); src->GetParent()->Close(); }, pokestring("CANCEL"), 0));
		select->UpdateMenu();

		this->GetMenu()->ShowTextbox(select, false);
	};

	this->party = party;
	menu->ClearItems();
	for (int i = 0; i < 6; i++)
	{
		string s = party[i]->nickname;
		while (s.length() < 10)
			s.insert(s.end(), MENU_BLANK);
		//now append the level
		if (party[i]->level < 100)
			s.insert(s.end(), 0xC); //L:
		s.append(pokestring(itos(party[i]->level)));
		s.insert(s.end(), MESSAGE_LINE);
		s.insert(s.end(), MENU_BLANK);
		s.insert(s.end(), 0xF); //H
		s.insert(s.end(), 0x00); //P:

		for (int h = 0; h < 6; h++)
		{
			s.insert(s.end(), MENU_BLANK);
		}
		s.insert(s.end(), 0x0A);
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

		menu->GetItems().push_back(new TextItem(menu, select, s, i, party[i]->id));
	}
	menu->UpdateMenu();
}

void PokemonInfo::DrawHPBars(sf::RenderWindow* window)
{
	sf::IntRect src_rect = sf::IntRect(0, 0, 8, 8);
	sf::Sprite sprite8x8;
	for (int i = 0; i < 6; i++)
	{
		DrawHPBar(window, sprite8x8, src_rect, 6, i * 2 + 1, party[i]);
	}
}

void PokemonInfo::DrawIcons(sf::RenderWindow* window)
{
	if (menu->GetArrowState() & ArrowStates::ACTIVE)
		selection_delay++;
	unsigned char pixels = CalculateHPBars(party[GetMenu()->GetActiveIndex()]->hp, party[GetMenu()->GetActiveIndex()]->max_hp);
	unsigned char reset_point = (pixels < 10 ? 50 : pixels < 27 ? 24 : 12);
	if (selection_delay >= reset_point || GetMenu()->GetActiveIndex() != last_hover)
		selection_delay = 0;
	last_hover = GetMenu()->GetActiveIndex();

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
				if (i == last_hover && selection_delay >= reset_point / 2)
					t += 0x40; //animate
				src_rect.left = (t % 16) * 8 + (mirrored && x == 1 ? 8 : 0);
				src_rect.top = (t / 16) * 8;
				src_rect.width = (mirrored && x == 1 ? -8 : 8);
				sprite8x8.setTextureRect(src_rect);

				dest_x = (int)(8 + x * 8 + (mirrored && x == 1 ? 1 - x : 0) * 8);
				dest_y = (int)(i * 16 + y * 8);
				if (i == last_hover && (c == 1 || c == 2) && selection_delay >= reset_point / 2)
					dest_y--;
				sprite8x8.setPosition((float)dest_x, (float)dest_y);
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

void PokemonInfo::SwapPokemon()
{
	if (!this->GetMenu()->IsSwitchCanceled())
	{
		Pokemon* from = this->GetParty()[this->GetMenu()->GetInactiveIndex()];
		this->GetParty()[this->GetMenu()->GetInactiveIndex()] = this->GetParty()[this->GetMenu()->GetActiveIndex()];
		this->GetParty()[this->GetMenu()->GetActiveIndex()] = from;
	}

	choose_textbox->ClearItems();
	choose_textbox->GetItems().push_back(new TextItem(choose_textbox, nullptr, pokestring("Choose a #MON.")));
	choose_textbox->UpdateMenu();
}

void PokemonInfo::DrawHPBar(sf::RenderWindow* window, sf::Sprite& sprite8x8, sf::IntRect& src_rect, int x, int y, Pokemon* p)
{
	src_rect.width = 8;
	src_rect.height = 8;
	unsigned char pixels = CalculateHPBars(p->hp, p->max_hp);
	if (pixels < 10)
		sprite8x8.setTexture(*ResourceCache::GetStatusesTexture(2));
	else if (pixels < 27)
		sprite8x8.setTexture(*ResourceCache::GetStatusesTexture(1));
	else
		sprite8x8.setTexture(*ResourceCache::GetStatusesTexture(0));
	for (int h = 0; h < 6; h++)
	{
		unsigned char c = 0x9; //full
		if (pixels == 0)
			c = 1;
		else if (pixels < 8)
		{
			c = pixels + 1;
			pixels = 0;
		}
		else
			pixels -= 8;
		src_rect.left = (c % 16) * 8;
		src_rect.top = c / 16 * 8;
		sprite8x8.setTextureRect(src_rect);
		sprite8x8.setPosition((h + x) * 8, y * 8);
		window->draw(sprite8x8);
	}
}

void PokemonInfo::DisplaySummary(Pokemon* p)
{
	Textbox* top = new Textbox(-1, 0, 22, 9, true, true);
	top->SetMenu(true, 4, sf::Vector2i(9, 0), sf::Vector2u(0, 0));
	top->SetRenderCallback([this](sf::RenderWindow* r) { sf::Sprite s; sf::IntRect ir; this->DrawHPBar(r, s, ir, 13, 3, this->GetParty()[this->GetMenu()->GetActiveIndex()]); });

	string s = p->nickname;
	s.append(pokestring("\n     "));
	if (p->level < 100)
		s.insert(s.end(), 0xC);
	s.append(pokestring(itos(p->level)));
	s.insert(s.end(), MESSAGE_LINE);
	s.append(pokestring("  "));
	s.insert(s.end(), 0xF); //H
	s.insert(s.end(), 0x00); //P:

	s.append(pokestring("\n   "));
	if (p->hp < 10)
		s.insert(s.end(), MENU_BLANK);
	if (p->hp < 100)
		s.insert(s.end(), MENU_BLANK);
	s.append(pokestring(itos(p->hp).append("/")));

	if (p->max_hp < 10)
		s.insert(s.end(), MENU_BLANK);
	if (p->max_hp < 100)
		s.insert(s.end(), MENU_BLANK);
	s.append(pokestring(itos(p->max_hp)));
	s.append(pokestring("\n\nSTATUS/OK"));

	top->GetItems().push_back(new TextItem(top, nullptr, s));
	top->UpdateMenu();

	for (int i = 0; i < 7; i++)
		top->GetTiles()[i * 20 - 1] = (i != 3 ? 0x21 : 0x1E);
	top->GetTiles()[6 * 20 + 8] = 0x20;
	for (int i = 0; i < 10; i++)
		top->GetTiles()[6 * 20 + 9 + i] = 0x24;
	top->GetTiles()[7 * 20 - 1] = 0x25;

	this->GetMenu()->ShowTextbox(top);



	Textbox* bottom = new Textbox(9, 8, 12, 10, true, true);
	bottom->SetMenu(true, 4, sf::Vector2i(0, 0), sf::Vector2u(0, 2));
	bottom->GetItems().push_back(new TextItem(bottom, nullptr, pokestring(string("TYPE1/\n ").append(Pokemon::GetTypeName(p->type1)))));
	bottom->GetItems().push_back(new TextItem(bottom, nullptr, (p->type2 != p->type1 ? pokestring(string("TYPE2/\n ").append(Pokemon::GetTypeName(p->type2))) : "")));
	s = 0x11;
	s.insert(s.end(), 0x12);
	s.append(pokestring("/\n  "));
	s.append(pokestring(itos(p->ot)));
	bottom->GetItems().push_back(new TextItem(bottom, nullptr, s));
	bottom->GetItems().push_back(new TextItem(bottom, nullptr, string(pokestring("OT/\n  ")).append(p->ot_name)));
	bottom->UpdateMenu();

	for (int i = 0; i < 8; i++)
		bottom->GetTiles()[i * 10 + 10 - 1] = 0x21;
	for (int i = 0; i < 6; i++)
		bottom->GetTiles()[8 * 10 + 3 + i] = 0x24;
	bottom->GetTiles()[8 * 10 + 2] = 0x20;
	bottom->GetTiles()[8 * 10 + 10 - 1] = 0x25;

	this->GetMenu()->ShowTextbox(bottom);


	Textbox* stats = new Textbox(0, 8, 10, 10);
	stats->SetMenu(true, 4, sf::Vector2i(0, 0), sf::Vector2u(0, 2));
	s = "ATTACK\n";
	for (int i = 0; i < 5 + (p->attack < 10 ? 2 : p->attack < 100 ? 1 : 0); i++)
		s.append(" ");
	s.append(itos(p->attack));
	stats->GetItems().push_back(new TextItem(stats, nullptr, pokestring(s)));

	s = "DEFENSE\n";
	for (int i = 0; i < 5 + (p->defense < 10 ? 2 : p->defense < 100 ? 1 : 0); i++)
		s.append(" ");
	s.append(itos(p->defense));
	stats->GetItems().push_back(new TextItem(stats, nullptr, pokestring(s)));

	s = "SPEED\n";
	for (int i = 0; i < 5 + (p->speed < 10 ? 2 : p->speed < 100 ? 1 : 0); i++)
		s.append(" ");
	s.append(itos(p->speed));
	stats->GetItems().push_back(new TextItem(stats, nullptr, pokestring(s)));

	s = "SPECIAL\n";
	for (int i = 0; i < 5 + (p->special < 10 ? 2 : p->special < 100 ? 1 : 0); i++)
		s.append(" ");
	s.append(itos(p->special));
	stats->GetItems().push_back(new TextItem(stats, nullptr, pokestring(s)));

	stats->UpdateMenu();
	this->GetMenu()->ShowTextbox(stats);
}