#include "ItemStorage.h"

ItemStorage::ItemStorage()
{
	for (int i = 0; i < MAX_ITEMS; i++)
		items.push_back(Item(i + 31, 1));

	menu = new Textbox(4, 2, 16, 11, false);
	menu->SetMenu(true, 4, sf::Vector2i(1, 1), sf::Vector2u(0, 2), []() { MenuCache::StartMenu()->SetArrowState(ArrowStates::ACTIVE); }, MenuFlags::FOCUSABLE, 2);
	menu->SetArrowState(ArrowStates::ACTIVE);
	
	GenerateItems();
}

ItemStorage::~ItemStorage()
{
	if (menu)
		delete menu;
}

void ItemStorage::GenerateItems()
{
	for (int i = 0; i < items.size(); i++)
	{
		if (items[i].id == 0 || items[i].quantity == 0)
			break;
		string s = ResourceCache::GetItemName(items[i].id);
		string amt = itos(items[i].quantity);
		if (amt.length() == 1)
			amt.insert(amt.begin(), ' ');
		if (!ResourceCache::IsKeyItem(items[i].id))
			s.append(pokestring("\n        *")).append(pokestring(amt));
		menu->GetItems().push_back(new TextItem(menu, nullptr, s, i, items[i].id));
	}
	menu->GetItems().push_back(new TextItem(menu, nullptr, pokestring("CANCEL")));
	menu->UpdateMenu();
}