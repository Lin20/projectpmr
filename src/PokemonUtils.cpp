#include "PokemonUtils.h"

void PokemonUtils::WriteStats(Textbox* t, Pokemon* p, char x)
{
	string s = "ATTACK\n";
	for (int i = 0; i < x; i++)
		s.insert(s.begin(), ' ');
	for (int i = 0; i < 5 + (p->attack < 10 ? 2 : p->attack < 100 ? 1 : 0); i++)
		s.insert(s.end(), ' ');
	s.append(itos(p->attack));

	t->GetItems().push_back(new TextItem(t, nullptr, pokestring(s)));

	s = "DEFENSE\n";
	for (int i = 0; i < x; i++)
		s.insert(s.begin(), ' ');
	for (int i = 0; i < 5 + (p->defense < 10 ? 2 : p->defense < 100 ? 1 : 0); i++)
		s.insert(s.end(), ' ');
	s.append(itos(p->defense));
	t->GetItems().push_back(new TextItem(t, nullptr, pokestring(s)));

	s = "SPEED\n";
	for (int i = 0; i < x; i++)
		s.insert(s.begin(), ' ');
	for (int i = 0; i < 5 + (p->speed < 10 ? 2 : p->speed < 100 ? 1 : 0); i++)
		s.insert(s.end(), ' ');
	s.append(itos(p->speed));
	t->GetItems().push_back(new TextItem(t, nullptr, pokestring(s)));

	s = "SPECIAL\n";
	for (int i = 0; i < x; i++)
		s.insert(s.begin(), ' ');
	for (int i = 0; i < 5 + (p->special < 10 ? 2 : p->special < 100 ? 1 : 0); i++)
		s.insert(s.end(), ' ');
	s.append(itos(p->special));

	t->GetItems().push_back(new TextItem(t, nullptr, pokestring(s)));

	t->UpdateMenu();
}

void PokemonUtils::Levelup(TextItem* src, Pokemon* p, std::function<void(TextItem*)> action_callback)
{
	auto stats_f = [p, src, action_callback](TextItem* s)
	{
		Textbox* stats = new Textbox(9, 2, 11, 10);
		auto close = [action_callback, stats](TextItem* z) {stats->Close(true); action_callback(z); };
		stats->SetMenu(true, 4, sf::Vector2i(0, 0), sf::Vector2u(0, 2), close, MenuFlags::FOCUSABLE, 2147u, nullptr, true, sf::Vector2i(-100, 0));
		PokemonUtils::WriteStats(stats, p, 1);
		for (int i = 0; i < 4; i++)
			stats->GetItems()[i]->SetAction(close);

		stats->SetArrowState(ArrowStates::ACTIVE);
		MenuCache::PokemonMenu()->GetChooseTextbox()->ShowTextbox(stats, false);

		for (int i = 0; i < 16; i++)
		{
			if (p->learnset[i].level == 0)
				break;
			if (p->learnset[i].level == p->level)
			{
				std::function<void(TextItem* t)> m_f = LearnMove(stats, p, p->learnset[i].move);
				for (int i = 0; i < 4; i++)
					stats->GetItems()[i]->SetAction(m_f);
				stats->SetTextTimer();
			}
		}
	};

	p->level++;
	unsigned short old_hp = p->max_hp;
	p->RecalculateStats();
	p->hp += p->max_hp - old_hp;
	MenuCache::PokemonMenu()->GetChooseTextbox()->GetItems()[0]->SetText(string(p->nickname).append(pokestring(" grew\n\nto level ").append(pokestring(itos(p->level).c_str())).append(pokestring("!\f"))));
	MenuCache::PokemonMenu()->UpdateOnePokemon(src->index);
	MenuCache::PokemonMenu()->GetMenu()->UpdateMenu();
	MenuCache::PokemonMenu()->GetChooseTextbox()->UpdateMenu();
	MenuCache::PokemonMenu()->FocusChooseTextbox();

	MenuCache::PokemonMenu()->GetChooseTextbox()->GetItems()[0]->SetAction(stats_f);
}

//src is the stat menu
std::function<void(TextItem* s)> PokemonUtils::LearnMove(Textbox* src, Pokemon* p, unsigned char move, bool close_src)
{
	for (int i = 0; i < 4; i++)
	{
		if (p->moves[i].index == 0)
		{
			p->moves[i] = Move(move);
			auto m_f = [src, p, i, close_src](TextItem* s_t)
			{
				Textbox* learned = new Textbox();
				learned->SetText(new TextItem(src, nullptr, string(p->nickname).append(pokestring(" learned\n").append(p->moves[i].name).append(pokestring("!\f")))));
				if (close_src)
				{
					learned->SetCloseCallback([src](TextItem* z){src->Close(); });
				}
				src->ShowTextbox(learned, false);
			};

			return m_f;
		}
	}

	//this was really annoying to add because of some changed the Textbox class needed.
	//i also got sick of having to use a TextItem parameter, so the names increment for simplicity.
	auto m_f = [src, p, move, close_src](TextItem* s_t)
	{
		Textbox* learned = new Textbox();

		auto yes_no = [learned, src, p, move, close_src](TextItem* s)
		{
			Textbox* yn = new Textbox(14, 7, 6, 5);
			auto no = [learned, yn, src, p, move, close_src](TextItem* s2)
			{
				Textbox* abandon = new Textbox();

				auto confirm_abandon = [abandon, learned, src, p, move, close_src, yn](TextItem* s3)
				{
					Textbox* yn2 = new Textbox(14, 7, 6, 5);
					auto reset = [yn2, src, learned, abandon, yn](TextItem* s3) {
						yn2->Close(true);
						abandon->Close();
						learned->Reset();
						src->GetItems()[0]->Action();
					};

					auto quit = [yn2, p, move, src, abandon, yn, learned](TextItem* s3)
					{
						yn2->Close(true);
						abandon->Close();
						Textbox* forgotten = new Textbox();
						forgotten->SetText(new TextItem(forgotten, [src, learned](TextItem* s4) { src->Close(true); MenuCache::PokemonMenu()->GetMenu()->Close(); }, string(p->nickname).append(pokestring("\ndid not learn\v")).append(Move(move).name).append(pokestring("!\f"))));
						src->ShowTextbox(forgotten, false);
					};

					yn2->SetMenu(true, 2, sf::Vector2i(1, 0), sf::Vector2u(0, 2), reset, MenuFlags::FOCUSABLE);
					yn2->SetArrowState(ArrowStates::ACTIVE);
					yn2->GetItems().push_back(new TextItem(yn2, quit, pokestring("YES"), 0));
					yn2->GetItems().push_back(new TextItem(yn2, reset, pokestring("NO"), 1));
					yn2->UpdateMenu();
					abandon->CancelClose();
					abandon->ShowTextbox(yn2, false);
				};

				yn->Close(true);
				abandon->SetText(new TextItem(abandon, confirm_abandon, pokestring("Abandon learning\n").append(Move(move).name).append(pokestring("?\a"))));
				learned->CancelClose();
				learned->ShowTextbox(abandon, false);
			};

			auto yes = [no, learned, yn, src, p, move, close_src](TextItem* s)
			{
				Textbox* which_move = new Textbox();

				auto show_moves = [no, which_move, src, p, move, close_src, learned](TextItem* s2)
				{
					Textbox* moves = new Textbox(4, 7, 16, 6);

					auto move_selected = [which_move, moves, src, p, move, learned](TextItem* s3)
					{
						moves->Close(true);
						which_move->Close(true);
						Textbox* last = new Textbox();
						last->SetText(new TextItem(last, [src, learned](TextItem* s4) { learned->Close(true); src->Close(true); MenuCache::PokemonMenu()->GetMenu()->Close(); }, pokestring("1, 2 and... \tPoof!\r").append(p->nickname).append(pokestring(" forgot\n")).append(p->moves[moves->GetActiveIndex()].name).append(pokestring("!\rAnd...\r").append(p->nickname).append(pokestring(" learned\n")).append(Move(move).name).append(pokestring("!\f")))));
						p->moves[moves->GetActiveIndex()] = Move(move);
						src->ShowTextbox(last, false);
					};

					moves->SetMenu(true, 4, sf::Vector2i(1, 0), sf::Vector2u(0, 1), [no, learned](TextItem* s4) {learned->Reset(); no(s4); }, MenuFlags::FOCUSABLE);
					moves->SetArrowState(ArrowStates::ACTIVE);
					for (unsigned int i = 0; i < 4; i++)
					{
						moves->GetItems().push_back(new TextItem(moves, move_selected, p->moves[i].name, i));
					}
					moves->UpdateMenu();
					which_move->CancelClose();
					which_move->ShowTextbox(moves, false);
				};

				yn->Close(true);
				which_move->SetText(new TextItem(which_move, show_moves, pokestring("Which move should\nbe forgotten?\a")));
				learned->CancelClose();
				learned->ShowTextbox(which_move, false);
			};
			yn->SetMenu(true, 2, sf::Vector2i(1, 0), sf::Vector2u(0, 2), no, MenuFlags::FOCUSABLE);
			yn->SetArrowState(ArrowStates::ACTIVE);
			yn->GetItems().push_back(new TextItem(yn, yes, pokestring("YES"), 0));
			yn->GetItems().push_back(new TextItem(yn, no, pokestring("NO"), 1));
			yn->UpdateMenu();
			learned->CancelClose();
			learned->ShowTextbox(yn, false);
		};

		string text = string(p->nickname).append(pokestring(" is\ntrying to learn\v").append(Move(move).name).append(pokestring("!\rBut, "))).append(p->nickname).append(pokestring("\ncan't learn more\vthan 4 moves!\rDelete an older\nmove to make room\vfor ")).append(Move(move).name).append(pokestring("?\a"));
		learned->SetText(new TextItem(src, yes_no, text));
		if (close_src)
		{
			learned->SetCloseCallback([src](TextItem* z){src->Close(); });
		}
		src->ShowTextbox(learned, false);
	};

	return m_f;
}