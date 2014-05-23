#include "PokemonInfo.h"

PokemonInfo::PokemonInfo()
{
	menu = 0;
	choose_textbox = 0;
	selection_delay = 0;
	delta_hp = 0;
	delta_hp_timer = 0;
	selected_bar_length = 0;
	for (int i = 0; i < 6; i++)
		ability[i] = false;

	//although the bottom textbox looks like an ordinary textbox,
	//the current textbox class doesn't support instant full printout of text.
	//so we can just fake it by making it a display-only menu
	choose_textbox = new Textbox(0, 12, 20, 6, false);
	choose_textbox->SetMenu(true, 1, sf::Vector2i(0, 1), sf::Vector2u(0, 2), nullptr, MenuFlags::FOCUSABLE, 2147U, nullptr, true, sf::Vector2i(-10, 0));
	choose_textbox->GetItems().push_back(new TextItem(choose_textbox, [this](TextItem* src) {this->GetChooseTextbox()->Close(); }, pokestring("Choose a #MON.")));
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
	{
		//menu->CloseAll();
		delete menu;
	}
	if (choose_textbox)
	{
		//choose_textbox->CloseAll();
		delete choose_textbox;
	}
}

void PokemonInfo::Show(Textbox* parent)
{
	choose_textbox->ClearItems();
	choose_textbox->GetItems().push_back(new TextItem(choose_textbox, nullptr, pokestring("Choose a #MON.")));
	choose_textbox->UpdateMenu();
	menu->SetCloseCallback([this](TextItem* src) { this->GetChooseTextbox()->Close(true); });
	menu->SetArrowState(ArrowStates::ACTIVE);
	parent->ShowTextbox(choose_textbox);
	parent->ShowTextbox(menu);
}

void PokemonInfo::Show(Textbox* parent, string text, std::function<void(TextItem* src)> select_callback, std::function<void(TextItem* src)> close_callback)
{
	this->parent = parent;
	this->GetChooseTextbox()->ClearItems();
	this->GetChooseTextbox()->GetItems().push_back(new TextItem(choose_textbox, nullptr, text));
	this->GetChooseTextbox()->UpdateMenu();
	this->GetMenu()->SetCloseCallback(close_callback);
	menu->SetArrowState(ArrowStates::ACTIVE);

	for (unsigned int i = 0; i < this->GetMenu()->GetItems().size(); i++)
	{
		if (this->GetMenu()->GetItems()[i])
			this->GetMenu()->GetItems()[i]->SetAction(select_callback);
	}
	parent->ShowTextbox(choose_textbox);
	parent->ShowTextbox(menu);
}

void PokemonInfo::FocusChooseTextbox()
{
	parent->GetTextboxes().erase(parent->GetTextboxes().begin() + parent->GetTextboxes().size() - 2);
	parent->GetTextboxes().push_back(choose_textbox);
	menu->SetArrowState(ArrowStates::HIDDEN);
	choose_textbox->GetItems()[0]->SetAction([this](TextItem* src) {this->GetChooseTextbox()->Close(); });
	choose_textbox->SetArrowState(ArrowStates::ACTIVE);
	choose_textbox->SetCloseCallback([this](TextItem* src) { menu->Close(); });
}

void PokemonInfo::UpdatePokemon(Pokemon** party, bool show_able_notable)
{
	this->show_able_notable = show_able_notable;
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
		menu->GetItems().push_back(new TextItem(menu, select, "", i, party[i]->id));
		UpdateOnePokemon((unsigned char)i, ability[i]);
	}
	menu->UpdateMenu();
}

void PokemonInfo::UpdateOnePokemon(unsigned char index, bool is_able)
{
	string s = party[index]->nickname;
	while (s.length() < 10)
		s.insert(s.end(), MENU_BLANK);
	//now append the level
	if (party[index]->level < 100)
		s.insert(s.end(), 0xC); //L:
	s.append(pokestring(itos(party[index]->level).c_str()));
	if (!show_able_notable)
	{
		if (party[index]->level < 10)
			s.insert(s.end(), MENU_BLANK);
		s.insert(s.end(), MENU_BLANK);
		if (party[index]->status != Statuses::OK)
			s.append(pokestring(party[index]->GetStatusName(party[index]->status)));
	}
	s.insert(s.end(), MESSAGE_LINE);

	if (!show_able_notable)
	{
		s.insert(s.end(), MENU_BLANK);
		s.insert(s.end(), 0xF); //H
		s.insert(s.end(), 0x00); //P:

		for (int h = 0; h < 6; h++)
		{
			s.insert(s.end(), MENU_BLANK);
		}
		s.insert(s.end(), 0x0A);
		if (party[index]->hp < 10)
			s.insert(s.end(), MENU_BLANK);
		if (party[index]->hp < 100)
			s.insert(s.end(), MENU_BLANK);
		s.append(pokestring(itos(party[index]->hp).append("/")));

		if (party[index]->max_hp < 10)
			s.insert(s.end(), MENU_BLANK);
		if (party[index]->max_hp < 100)
			s.insert(s.end(), MENU_BLANK);
		s.append(pokestring(itos(party[index]->max_hp).c_str()));
	}
	else
	{
		s.append(pokestring("         "));
		if (is_able)
			s.append(pokestring("ABLE"));
		else
			s.append(pokestring("NOT ABLE"));

	}
	menu->GetItems()[index]->SetText(s);
}

void PokemonInfo::DrawHPBars(sf::RenderWindow* window)
{
	if (show_able_notable)
		return;
	sf::IntRect src_rect = sf::IntRect(0, 0, 8, 8);
	sf::Sprite sprite8x8;
	for (int i = 0; i < 6; i++)
	{
		unsigned int pixels = CalculateHPBars(party[i]->hp, party[i]->max_hp);
		if (i == menu->GetActiveIndex() && delta_hp != 0)
		{
			pixels = selected_bar_length;
			if (delta_hp_timer > 0)
			{
				delta_hp_timer--;
			}
			else
			{
				delta_hp_timer = 1;
				unsigned int difference = CalculateHPBars(party[i]->hp + (delta_hp > 0 ? 1 : -1), party[i]->max_hp) - pixels;
				if (!difference)
				{
					party[i]->hp += (delta_hp > 0 ? 1 : -1);
					hp_amount_changed += (delta_hp > 0 ? 1 : -1);
					UpdateOnePokemon(i);
					menu->UpdateMenu();
					delta_hp -= (delta_hp > 0 ? 1 : -1);
					if (party[i]->hp == 0 || party[i]->hp == party[i]->max_hp || delta_hp == 0)
					{
						delta_hp = 0;
						if (heal_callback != nullptr)
							heal_callback(menu->GetItems()[menu->GetActiveIndex()]);
					}
				}
				else
				{
					selected_bar_length += (delta_hp > 0 ? 1 : -1);
					pixels = selected_bar_length;
				}
			}
		}
		DrawHPBar(window, sprite8x8, src_rect, 6, i * 2 + 1, party[i], pixels);
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
				src_rect.height = 8;

				dest_x = (int)(8 + x * 8 + (mirrored && x == 1 ? 1 - x : 0) * 8);
				dest_y = (int)(i * 16 + y * 8);
				if (i == last_hover && (c == 1 || c == 2) && selection_delay >= reset_point / 2)
				{
					//this used to just move the ball/shell up one pixel
					//however, some sprites above it will get cut off at the bottom
					//so we need to keep it positioned in the same place but move the tile src up
					if (y == 0)
					{
						src_rect.top++;
						src_rect.height--;
					}
					else
						dest_y--;
				}

				sprite8x8.setTextureRect(src_rect);
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

void PokemonInfo::DrawHPBar(sf::RenderWindow* window, sf::Sprite& sprite8x8, sf::IntRect& src_rect, int x, int y, Pokemon* p, unsigned int pixels)
{
	src_rect.width = 8;
	src_rect.height = 8;
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
		sprite8x8.setPosition((float)((h + x) * 8), (float)(y * 8));
		window->draw(sprite8x8);
	}
}

void PokemonInfo::Heal(int amount, std::function<void(TextItem* src)> f)
{
	if (f)
		this->heal_callback = f;
	else
		this->heal_callback = nullptr;
	hp_amount_changed = 0;
	unsigned int index = menu->GetActiveIndex();
	menu->SetArrowState(ArrowStates::INACTIVE);
	delta_hp = amount;
	delta_hp_timer = 2;
	selected_bar_length = CalculateHPBars(party[menu->GetActiveIndex()]->hp, party[menu->GetActiveIndex()]->max_hp);
}

void PokemonInfo::DisplaySummary(Pokemon* p)
{
	Textbox* top = new Textbox(-1, 0, 22, 11, true, true);
	top->SetMenu(true, 4, sf::Vector2i(9, 0), sf::Vector2u(0, 0));
	top->SetRenderCallback([this](sf::RenderWindow* r) {
		sf::Sprite s;
		sf::IntRect ir;
		if (this->GetMenu()->GetTextboxes().size() > 3)
		{
			this->DrawHPBar(r, s, ir, 13, 3, this->GetParty()[this->GetMenu()->GetActiveIndex()], CalculateHPBars(party[menu->GetActiveIndex()]->hp, party[menu->GetActiveIndex()]->max_hp));
		}
		s.setTexture(*ResourceCache::GetPokemonFront(this->GetParty()[this->GetMenu()->GetActiveIndex()]->pokedex_index), true);
		ir.left = this->GetParty()[this->GetMenu()->GetActiveIndex()]->size_x * 8;
		ir.top = 0;
		ir.width = -this->GetParty()[this->GetMenu()->GetActiveIndex()]->size_x * 8;
		ir.height = this->GetParty()[this->GetMenu()->GetActiveIndex()]->size_y * 8;
		s.setTextureRect(ir);
		s.setPosition((float)(64 + ir.width), (float)(56 - ir.height));
		r->draw(s);
	});

	string s = p->nickname;
	s.append(pokestring("\n     "));
	if (p->level < 100)
		s.insert(s.end(), 0xC);
	s.append(pokestring(itos(p->level).c_str()));
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
	s.append(pokestring(itos(p->max_hp).c_str()));
	s.append(pokestring("\n\nSTATUS/").append(pokestring(Pokemon::GetStatusName(p->status))).append(pokestring("  ")));
	s.insert(s.end(), 0x12); //No
	s.append(pokestring(string(".").append(p->pokedex_index < 100 ? "0" : "").append(p->pokedex_index < 10 ? "0" : "").append(itos(p->pokedex_index)).c_str()));

	top->GetItems().push_back(new TextItem(top, nullptr, s));
	top->UpdateMenu();

	for (int i = 1; i < 7; i++)
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
	s.append(pokestring(itos(p->ot).c_str()));
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


	auto f = [this](TextItem* src) { this->DisplaySummary2(this->GetParty()[this->GetMenu()->GetActiveIndex()]); };
	Textbox* stats = new Textbox(0, 8, 10, 10);
	stats->SetMenu(true, 4, sf::Vector2i(0, 0), sf::Vector2u(0, 2), f, MenuFlags::FOCUSABLE, 2147u, nullptr, true, sf::Vector2i(-100, 0));
	PokemonUtils::WriteStats(stats, p, 0);

	for (unsigned int i = 0; i < stats->GetItems().size(); i++)
		stats->GetItems()[i]->SetAction(f);
	stats->SetArrowState(ArrowStates::ACTIVE);
	this->GetMenu()->ShowTextbox(stats);
}

void PokemonInfo::DisplaySummary2(Pokemon* p)
{
	this->GetMenu()->GetTextboxes()[this->GetMenu()->GetTextboxes().size() - 1]->Close(true);
	this->GetMenu()->GetTextboxes()[this->GetMenu()->GetTextboxes().size() - 2]->Close(true);

	Textbox* top = this->GetMenu()->GetTextboxes()[1];
	top->ClearItems();
	string s = p->original_name;
	s.append(pokestring("\n\nEXP POINTS\n   "));
	unsigned int digits = (p->xp > 0 ? (unsigned int)floor(log10(p->xp) + 1) : 1);
	for (unsigned int i = 0; i < 7 - digits; i++)
		s.insert(s.end(), MENU_BLANK);
	s.append(pokestring(itos(p->xp).c_str()));
	s.append(pokestring("\nLEVEL UP\n"));
	digits = (p->GetXPRemaining() > 0 ? (unsigned int)floor(log10(p->GetXPRemaining())) : 1);
	for (unsigned int i = 0; i < 4 - digits; i++)
		s.insert(s.end(), MENU_BLANK);
	s.append(pokestring(itos(p->GetXPRemaining()).c_str()));
	s.insert(s.end(), 14); //to
	s.insert(s.end(), MENU_BLANK);
	unsigned char l = (p->level < 100 ? p->level + 1 : 100);
	if (l < 100)
		s.insert(s.end(), 0xC); //L:
	s.append(pokestring(itos(l).c_str()));
	if (l < 10)
		s.insert(s.end(), MENU_BLANK);
	s.insert(s.end(), MENU_BLANK);
	s.insert(s.end(), MENU_BLANK);

	s.insert(s.end(), 0x12);
	s.append(pokestring(string(".").append(p->pokedex_index < 100 ? "0" : "").append(p->pokedex_index < 10 ? "0" : "").append(itos(p->pokedex_index)).c_str()));

	top->GetItems().push_back(new TextItem(top, nullptr, s));
	top->UpdateMenu();

	for (int i = 1; i < 7; i++)
		top->GetTiles()[i * 20 - 1] = 0x21;
	top->GetTiles()[6 * 20 + 8] = 0x20;
	for (int i = 0; i < 10; i++)
		top->GetTiles()[6 * 20 + 9 + i] = 0x24;
	top->GetTiles()[7 * 20 - 1] = 0x25;

	auto f = [this](TextItem* src)
	{
		this->GetMenu()->GetTextboxes()[this->GetMenu()->GetTextboxes().size() - 1]->Close(true);
		this->GetMenu()->GetTextboxes()[this->GetMenu()->GetTextboxes().size() - 2]->Close(true);
		this->GetMenu()->GetTextboxes()[this->GetMenu()->GetTextboxes().size() - 3]->Close(true);
		this->GetMenu()->SetArrowState(ArrowStates::ACTIVE);
		this->GetMenu()->SetJustOpened();
		this->GetChooseTextbox()->SetJustOpened();
	};

	Textbox* moves = new Textbox(0, 8, 20, 10);
	moves->SetMenu(true, 4, sf::Vector2i(1, 0), sf::Vector2u(0, 2), f, MenuFlags::FOCUSABLE, 2147u, nullptr, true, sf::Vector2i(-2, 0));
	moves->SetArrowState(ArrowStates::ACTIVE);
	for (int i = 0; i < 4; i++)
	{
		string s = p->moves[i].name;
		s.append(pokestring("\n         "));
		if (p->moves[i].index > 0)
		{
			s.append(pokestring("PP "));
			if (p->moves[i].pp < 10)
				s.insert(s.end(), MENU_BLANK);
			s.append(pokestring(itos(p->moves[i].pp).c_str()));
			s.append(pokestring("/"));
			if (p->moves[i].max_pp < 10)
				s.insert(s.end(), MENU_BLANK);
			s.append(pokestring(itos(p->moves[i].max_pp).c_str()));
		}
		else
			s.append(pokestring("--"));
		moves->GetItems().push_back(new TextItem(moves, f, s));
	}
	moves->UpdateMenu();

	this->GetMenu()->ShowTextbox(moves, false);
}
