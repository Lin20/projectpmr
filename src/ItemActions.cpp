#include "ItemActions.h"

ItemStorage* ItemActions::last_inventory = 0;
TextItem* ItemActions::last_src = 0;
unsigned char ItemActions::last_id = 0;

void ItemActions::UseItem(ItemStorage* inventory, TextItem* src, unsigned char id)
{
	last_inventory = inventory;
	last_src = src;
	last_id = id;

	unsigned char usage = ResourceCache::GetItemUse(id);
	//anything 0x3F and under is outside-only
	//anything between 0x40 and 0x7F can be used both places
	//anything between 0x80 and 0xBF is battle-only
	//anything 0xC0 and above is safari-only
	unsigned char state = Engine::GetState();

	if (usage <= 0x3F && state != States::OVERWORLD)
		usage = 0;
	else if (usage >= 0x40 && usage <= 0x7F && state == States::SAFARIBATTLE)
		usage = 0;
	else if (usage >= 0x80 && usage < 0xC0 && state != States::BATTLE)
		usage = 0;
	else if (usage >= 0xC0 && state != States::SAFARIBATTLE)
		usage = 0;

	auto close_pokemon = [inventory](TextItem* src)
	{
		if (!MenuCache::PokemonMenu()->GetChooseTextbox()->SetToClose())
			MenuCache::PokemonMenu()->GetChooseTextbox()->Close();
		inventory->GetMenu()->GetTextboxes()[0]->Close();
	};

	Textbox* t;
	string s;
	switch (usage)
	{
	case 0:
		t = new Textbox();
		s = string(pokestring("OAK: ")).append(inventory->GetOwner()->GetName()).append(pokestring("!\nThis isn't the\vtime to use that!\f"));
		t->SetText(new TextItem(t, [inventory](TextItem* src)
		{
			inventory->GetMenu()->GetTextboxes()[0]->Close();
		}, s));
		src->GetParent()->ShowTextbox(t, false);
		break;

	case 0x40: //medicine
		MenuCache::PokemonMenu()->UpdatePokemon(Players::GetPlayer1()->GetParty());
		MenuCache::PokemonMenu()->Show(src->GetParent(), pokestring("Use item on which\n\n#MON?"), UseMedicine, close_pokemon);
		break;

	default:
		t = new Textbox();
		s = pokestring("Sorry, that item\nhasn't been\vimplemented yet.");
		t->SetText(new TextItem(t, [inventory](TextItem* src)
		{
			inventory->GetMenu()->GetTextboxes()[0]->Close();
		}, s));
		src->GetParent()->ShowTextbox(t, false);
		break;
	}
}

void ItemActions::UseMedicine(TextItem* src)
{
	Pokemon* p = MenuCache::PokemonMenu()->GetParty()[src->index];
	ItemStorage* items = last_inventory;
	switch (last_id)
	{
	case 0x0B: //antidote
		if (p->status != Statuses::POISONED)
			break;
		p->status = Statuses::OK;
		MenuCache::PokemonMenu()->GetChooseTextbox()->GetItems()[0]->SetText(string(p->nickname).append(pokestring(" was\n\ncured of poison!")));
		MenuCache::PokemonMenu()->UpdatePokemon(MenuCache::PokemonMenu()->GetParty());
		MenuCache::PokemonMenu()->GetChooseTextbox()->UpdateMenu();
		MenuCache::PokemonMenu()->FocusChooseTextbox();
		last_inventory->RemoveItemFromSlot(last_src->index, 1);
		return;

	case 0x0C: //burn heal
		if (p->status != Statuses::BURNED)
			break;
		p->status = Statuses::OK;
		MenuCache::PokemonMenu()->GetChooseTextbox()->GetItems()[0]->SetText(string(p->nickname).append(pokestring("'s\n\nburn was healed!")));
		MenuCache::PokemonMenu()->UpdatePokemon(MenuCache::PokemonMenu()->GetParty());
		MenuCache::PokemonMenu()->GetChooseTextbox()->UpdateMenu();
		MenuCache::PokemonMenu()->FocusChooseTextbox();
		last_inventory->RemoveItemFromSlot(last_src->index, 1);
		return;

	case 0x0D: //ice heal
		if (p->status != Statuses::FROZEN)
			break;
		p->status = Statuses::OK;
		MenuCache::PokemonMenu()->GetChooseTextbox()->GetItems()[0]->SetText(string(p->nickname).append(pokestring(" was\n\ndefrosted!")));
		MenuCache::PokemonMenu()->UpdatePokemon(MenuCache::PokemonMenu()->GetParty());
		MenuCache::PokemonMenu()->GetChooseTextbox()->UpdateMenu();
		MenuCache::PokemonMenu()->FocusChooseTextbox();
		last_inventory->RemoveItemFromSlot(last_src->index, 1);
		return;

	case 0x0E: //awakening
		if (p->status != Statuses::SLEEPING)
			break;
		p->status = Statuses::OK;
		MenuCache::PokemonMenu()->GetChooseTextbox()->GetItems()[0]->SetText(string(p->nickname).append(pokestring("\n\nwoke up!")));
		MenuCache::PokemonMenu()->UpdatePokemon(MenuCache::PokemonMenu()->GetParty());
		MenuCache::PokemonMenu()->GetChooseTextbox()->UpdateMenu();
		MenuCache::PokemonMenu()->FocusChooseTextbox();
		last_inventory->RemoveItemFromSlot(last_src->index, 1);
		return;

	case 0x0F: //paralyze heal
		if (p->status != Statuses::PARALYZED)
			break;
		p->status = Statuses::OK;
		MenuCache::PokemonMenu()->GetChooseTextbox()->GetItems()[0]->SetText(string(p->nickname).append(pokestring("'s\n\nrid of paralysis!")));
		MenuCache::PokemonMenu()->UpdatePokemon(MenuCache::PokemonMenu()->GetParty());
		MenuCache::PokemonMenu()->GetChooseTextbox()->UpdateMenu();
		MenuCache::PokemonMenu()->FocusChooseTextbox();
		last_inventory->RemoveItemFromSlot(last_src->index, 1);
		return;

	case 0x34: //full heal
		if (p->status == Statuses::OK)
			break;
		p->status = Statuses::OK;
		MenuCache::PokemonMenu()->GetChooseTextbox()->GetItems()[0]->SetText(string(p->nickname).append(pokestring("'s\n\nheatl returned!")));
		MenuCache::PokemonMenu()->UpdatePokemon(MenuCache::PokemonMenu()->GetParty());
		MenuCache::PokemonMenu()->GetChooseTextbox()->UpdateMenu();
		MenuCache::PokemonMenu()->FocusChooseTextbox();
		last_inventory->RemoveItemFromSlot(last_src->index, 1);
		return;

	case 0x14: //potion
		if (p->hp == p->max_hp)
			break;
		MenuCache::PokemonMenu()->Heal(20);
		return;
	}

	Textbox* t = new Textbox();
	t->SetText(new TextItem(src->GetParent(), [items](TextItem* s){MenuCache::PokemonMenu()->GetMenu()->Close(); }, pokestring("It won't have any\n\neffect.")));
	//t->SetText(pokestring("It won't have any\n\neffect."));
	src->GetParent()->ShowTextbox(t, false);
}
