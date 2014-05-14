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