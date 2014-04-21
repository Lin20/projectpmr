#include "ItemStorage.h"

ItemStorage::ItemStorage()
{
	for (int i = 0; i < MAX_ITEMS; i++)
		items.push_back(Item((i < 15 ? i + 1 : 0), 1));

	menu = new Textbox(4, 2, 16, 11, false);
	menu->SetMenu(true, 4, sf::Vector2i(1, 1), sf::Vector2u(0, 2), []() { MenuCache::StartMenu()->SetArrowState(ArrowStates::ACTIVE); }, MenuFlags::FOCUSABLE | MenuFlags::HOLD_INPUT, 2);
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
	menu->ClearItems();
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

bool ItemStorage::AddItem(unsigned char id, unsigned char quantity)
{
	unsigned char room_for = 0;
	for (int i = 0; i < MAX_ITEMS && room_for < quantity; i++)
	{
		if (items[i].Empty())
			room_for += 99;
		else if (items[i].id == id)
			room_for += 99 - items[i].quantity;
	}
	if (room_for < quantity)
		return false;

	for (int i = 0; i < MAX_ITEMS; i++)
	{
		if (items[i].Empty())
		{
			items[i].id = id;
			items[i].quantity = min(quantity, (unsigned char)99);
			quantity -= min(quantity, (unsigned char)99);
			if (quantity == 0)
				break;
			continue;
		}
		else if (items[i].id == id)
		{
			unsigned char max_adding = 99 - items[i].quantity;
			items[i].quantity += min(quantity, max_adding);
			quantity -= min(quantity, max_adding);
			if (quantity == 0)
				break;
		}
	}

	GenerateItems();

	return true;
}