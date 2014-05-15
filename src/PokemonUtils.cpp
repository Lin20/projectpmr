#include "PokemonUtils.h"

void PokemonUtils::WriteStats(Textbox* t, Pokemon* p, char x)
{
	string s = "ATTACK\n";
	for (int i = 0; i < x; i++)
		s.insert(s.begin(), ' ');
	for (int i = 0; i < 5 + x + (p->attack < 10 ? 2 : p->attack < 100 ? 1 : 0); i++)
		s.append(" ");
	s.append(itos(p->attack));

	t->GetItems().push_back(new TextItem(t, nullptr, pokestring(s)));

	s = "DEFENSE\n";
	for (int i = 0; i < x; i++)
		s.insert(s.begin(), ' ');
	for (int i = 0; i < 5 + x + (p->defense < 10 ? 2 : p->defense < 100 ? 1 : 0); i++)
		s.append(" ");
	s.append(itos(p->defense));
	t->GetItems().push_back(new TextItem(t, nullptr, pokestring(s)));

	s = "SPEED\n";
	for (int i = 0; i < x; i++)
		s.insert(s.begin(), ' ');
	for (int i = 0; i < 5 + x + (p->speed < 10 ? 2 : p->speed < 100 ? 1 : 0); i++)
		s.append(" ");
	s.append(itos(p->speed));
	t->GetItems().push_back(new TextItem(t, nullptr, pokestring(s)));

	s = "SPECIAL\n";
	for (int i = 0; i < x; i++)
		s.insert(s.begin(), ' ');
	for (int i = 0; i < 5 + x + (p->special < 10 ? 2 : p->special < 100 ? 1 : 0); i++)
		s.append(" ");
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
}