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

		bool learned_move = false;
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
				learned_move = true;
			}
		}

		if (!learned_move)
		{
			for (int i = 0; i < 5; i++)
			{
				if (p->evolutions[i].trigger != 0)
				{
					if (p->evolutions[i].trigger == EVOLUTION_LEVEL && p->evolutions[i].level <= p->level)
					{
						auto m_f = Evolve(stats, p, p->evolutions[i].pokemon);
						for (int i = 0; i < 4; i++)
							stats->GetItems()[i]->SetAction(m_f);
						stats->SetTextTimer();
						learned_move = true;
						break;
					}
				}
				else
					break;
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
		if (p->moves[i].index == move)
			return [src](TextItem* s) { src->Close(); };
		if (p->moves[i].index == 0)
		{
			p->moves[i] = Move(move);
			auto m_f = [src, p, i, close_src](TextItem* s_t)
			{
				Textbox* learned = new Textbox();

				std::function<void(TextItem* s)> m_f = nullptr;
				for (int k = 0; k < 5; k++)
				{
					if (p->evolutions[k].trigger != 0)
					{
						if (p->evolutions[k].trigger == EVOLUTION_LEVEL && p->evolutions[k].level <= p->level)
						{
							m_f = Evolve(src, p, p->evolutions[k].pokemon);
							break;
						}
					}
					else
						break;
				}

				learned->SetText(new TextItem(src, m_f, string(p->nickname).append(pokestring(" learned\n").append(p->moves[i].name).append(pokestring("!\f")))));

				if (close_src && !m_f)
				{
					learned->SetCloseCallback([src](TextItem* z)
					{
						src->Close();
					});
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
						if (src->GetItems().size() > 0)
							src->GetItems()[0]->Action();
						else
							src->GetText()->Action();
					};

					auto quit = [yn2, p, move, src, abandon, yn, learned](TextItem* s3)
					{
						yn2->Close(true);
						abandon->Close();
						Textbox* forgotten = new Textbox();

						std::function<void(TextItem* s)> e_f = CheckMove(src, p);
						
						forgotten->SetText(new TextItem(forgotten, (!e_f ? [src, learned](TextItem* s4) { src->Close(true); MenuCache::PokemonMenu()->GetMenu()->Close(); } : e_f), string(p->nickname).append(pokestring("\ndid not learn\v")).append(Move(move).name).append(pokestring("!\f"))));
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
						Textbox* last = new Textbox();

						std::function<void(TextItem* s)> e_f = CheckMove(src, p);
						std::function<void(TextItem* s)> real_ef = [src, learned, which_move, e_f](TextItem* s4) {learned->Close(true); which_move->Close(true); e_f(s4); };

						last->SetText(new TextItem(last, (!e_f ? [src, learned, which_move](TextItem* s4) { learned->Close(true); which_move->Close(true); src->Close(); MenuCache::PokemonMenu()->GetMenu()->Close(); } : real_ef), pokestring("1, 2 and... \tPoof!\r").append(p->nickname).append(pokestring(" forgot\n")).append(p->moves[moves->GetActiveIndex()].name).append(pokestring("!\rAnd...\r").append(p->nickname).append(pokestring(" learned\n")).append(Move(move).name).append(pokestring("!\f")))));
						p->moves[moves->GetActiveIndex()] = Move(move);
						which_move->ShowTextbox(last, false);
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

std::function<void(TextItem* s)> PokemonUtils::Evolve(Textbox* src, Pokemon* p, unsigned char evolution, bool close_src)
{
	auto mf = [src, p, evolution, close_src](TextItem* s)
	{
		src->CancelClose();
		Textbox* what = new Textbox();

		auto start_evolution = [what, p, src, evolution](TextItem* s2)
		{
			what->CancelClose();
			EvolutionScreen* ev = new EvolutionScreen(p, evolution);
			ev->Show(what);
			what->SetCloseCallback([src, what, ev](TextItem* s3)
			{
				delete ev;
				what->Close(true);
				//src->CloseAll();
				src->Close(true);
				MenuCache::PokemonMenu()->GetMenu()->Close();
			});
		};

		what->SetText(new TextItem(what, start_evolution, pokestring("What? ").append(p->nickname).append(pokestring("\nis evolving!\t\t\a"))));
		what->SetArrowState(ArrowStates::INACTIVE);
		src->ShowTextbox(what, false);
	};

	return mf;
}

std::function<void(TextItem* s)> PokemonUtils::CheckMove(Textbox* src, Pokemon* p)
{
	for (int k = 0; k < 5; k++)
	{
		if (p->evolutions[k].trigger != 0)
		{
			if (p->evolutions[k].trigger == EVOLUTION_LEVEL &&p->evolutions[k].level <= p->level)
			{
				return Evolve(src, p, p->evolutions[k].pokemon);
				break;
			}
		}
		else
			break;
	}

	return nullptr;
}

void PokemonUtils::Faint(MapScene* scene, Textbox* t)
{
	t->GetText()->SetAction([t, scene](TextItem* src)
	{
		Textbox* f = new Textbox();
		f->SetText(new TextItem(f, [scene](TextItem* s2)
		{
			Warp w;
			w.dest_map = scene->GetLastHealedMap();
			for (int i = 0; i < 13; i++)
			{
				if (ResourceCache::GetFlyPoint(i).map == w.dest_map)
				{
					w.x = ResourceCache::GetFlyPoint(i).x;
					w.y = ResourceCache::GetFlyPoint(i).y;
					w.type = WARP_TYPE_SET;
					break;
				}
			}
			scene->WarpTo(w);

			for (unsigned int i = 0; i < Players::GetPlayer1()->GetPartyCount(); i++)
			{
				Players::GetPlayer1()->GetParty()[i]->Heal();
			}
			Players::GetPlayer1()->SetMoney(Players::GetPlayer1()->GetMoney() / 2);
			scene->GetEntities()[0]->SetSprite(Players::GetPlayer1()->GetOptions().player_sprite);

		}, string(Players::GetPlayer1()->GetName()).append(pokestring(" is out of\nuseable #MON!\r")).append(Players::GetPlayer1()->GetName()).append(pokestring(" blacked\nout!\f"))));
		scene->ShowTextbox(f, true);
	});
}
