#include "ItemStorage.h"

ItemStorage::ItemStorage(PlayerProperties* owner)
{
	this->owner = owner;
	for (int i = 0; i < MAX_ITEMS; i++)
		items.push_back(Item(0x28, 99));

	auto switch_items = [this]()
	{
		if (this->GetMenu()->GetActiveIndex() == this->GetItemCount())
		{
			this->GetMenu()->CancelSwitch();
		}
		else
		{
			Item i = this->GetItems()[this->GetMenu()->GetActiveIndex()];
			this->GetItems()[this->GetMenu()->GetActiveIndex()] = this->GetItems()[this->GetMenu()->GetInactiveIndex()];
			this->GetItems()[this->GetMenu()->GetInactiveIndex()] = i;
		}
	};

	menu = new Textbox(4, 2, 16, 11, false);
	menu->SetMenu(true, 4, sf::Vector2i(1, 1), sf::Vector2u(0, 2), [](TextItem* source) { MenuCache::StartMenu()->SetArrowState(ArrowStates::ACTIVE); }, MenuFlags::FOCUSABLE | MenuFlags::HOLD_INPUT | MenuFlags::SWITCHABLE, 2, switch_items, false);
	menu->SetArrowState(ArrowStates::ACTIVE);

	GenerateItems();
}

ItemStorage::~ItemStorage()
{
	if (menu)
		delete menu;
}

/*
 * yo dawg
 * i herd you like lambdas
 * so i wrote some lambdas in your lambdas
 * so you can lambda while you lambda
 *
 * Also TODO: Optimize.....
 */
void ItemStorage::GenerateItems()
{
	auto select = [this](TextItem* item)
	{
		auto toss = [this](TextItem* item)
		{
			if (ResourceCache::IsKeyItem(item->value))
			{
				Textbox* t = new Textbox();
				t->SetText(new TextItem(t, [this](TextItem*){this->GetMenu()->SetArrowState(ArrowStates::ACTIVE); this->GetMenu()->CloseAll(); }, pokestring("That's too impor-\ntant to toss!\f")));
				this->GetMenu()->ShowTextbox(t, false);
			}
			else
			{
				auto confirm = [this]()
				{
					auto confirm_toss_b = [this](TextItem* from)
					{
						auto close_all = [this](TextItem* yesnosrc)
						{
							this->GetMenu()->FlashCursor();
							this->GetMenu()->SetArrowState(ArrowStates::ACTIVE);
							this->GetMenu()->CloseAll();
						};

						auto toss_final = [this](TextItem* yesnosrc)
						{
							Textbox* threwaway = new Textbox();
							threwaway->SetText(new TextItem(threwaway, [this](TextItem* src) {
								this->RemoveItemFromSlot(this->GetMenu()->GetInactiveIndex(), this->GetMenu()->GetTextboxes()[1]->GetCounterValue()); this->GetMenu()->ResetSelection(); this->GetMenu()->SetArrowState(ArrowStates::ACTIVE); this->GetMenu()->CloseAll();
							}, pokestring("Threw away\n").append(ResourceCache::GetItemName(this->GetItems()[this->GetMenu()->GetInactiveIndex()].id)).append(pokestring(".\f"))));
							this->GetMenu()->ShowTextbox(threwaway, false);
						};

						from->GetParent()->CancelClose();
						Textbox* yesno = new Textbox(14, 7, 6, 5);
						yesno->SetMenu(true, 2, sf::Vector2i(1, 0), sf::Vector2u(0, 2), close_all, MenuFlags::FOCUSABLE);
						yesno->SetArrowState(ArrowStates::ACTIVE);
						yesno->GetItems().push_back(new TextItem(yesno, toss_final, pokestring("YES")));
						yesno->GetItems().push_back(new TextItem(yesno, close_all, pokestring("NO")));
						yesno->UpdateMenu();
						from->GetParent()->ShowTextbox(yesno, false);
					};
					Textbox* tb = new Textbox();
					tb->SetText(new TextItem(tb, confirm_toss_b, pokestring("Is it OK to toss\n").append(ResourceCache::GetItemName(this->GetItems()[this->GetMenu()->GetInactiveIndex()].id)).append(pokestring("?\f"))));
					this->GetMenu()->ShowTextbox(tb, false);
				};
				auto close_all = [this](TextItem* yesnosrc)
				{
					this->GetMenu()->FlashCursor();
					this->GetMenu()->SetArrowState(ArrowStates::ACTIVE);
					this->GetMenu()->CloseAll();
				};

				Textbox* t = new Textbox(15, 9, 5, 3);
				t->SetCounter(true, 1, this->GetItems()[item->index].quantity, confirm, close_all);
				this->GetMenu()->ShowTextbox(t, false);
				this->GetMenu()->GetTextboxes()[0]->SetArrowState(ArrowStates::INACTIVE);
			}
		};

		auto use = [this](TextItem* item)
		{
			item->GetParent()->SetArrowState(ArrowStates::INACTIVE);
			ItemActions::UseItem(this, item, item->value);
			/*Textbox* t = new Textbox();
			t->SetText(new TextItem(t, [this](TextItem* src)
			{
				this->GetMenu()->GetTextboxes()[0]->Close();
			}, pokestring("You just used\n").append(ResourceCache::GetItemName(item->value)).append(pokestring("."))));
			item->GetParent()->ShowTextbox(t, false);*/
		};

		this->GetMenu()->SetArrowState(ArrowStates::INACTIVE);

		Textbox* usetoss = new Textbox(13, 10, 7, 5);
		usetoss->SetMenu(true, 2, sf::Vector2i(1, 0), sf::Vector2u(0, 2), [this](TextItem* source){ this->GetMenu()->FlashCursor(); this->GetMenu()->SetArrowState(ArrowStates::ACTIVE); }, MenuFlags::FOCUSABLE);
		usetoss->GetItems().push_back(new TextItem(usetoss, use, pokestring("USE"), this->GetMenu()->GetInactiveIndex(), this->GetItems()[this->GetMenu()->GetInactiveIndex()].id));

		usetoss->GetItems().push_back(new TextItem(usetoss, toss, pokestring("TOSS"), this->GetMenu()->GetInactiveIndex(), this->GetItems()[this->GetMenu()->GetInactiveIndex()].id));
		usetoss->UpdateMenu();
		usetoss->SetArrowState(ArrowStates::ACTIVE);
		menu->ShowTextbox(usetoss, false);
	};

	menu->ClearItems();
	for (unsigned int i = 0; i < items.size(); i++)
	{
		if (items[i].id == 0 || items[i].quantity == 0)
			break;

		string s = ResourceCache::GetItemName(items[i].id);
		string amt = itos(items[i].quantity);
		if (amt.length() == 1)
			amt.insert(amt.begin(), ' ');
		if (!ResourceCache::IsKeyItem(items[i].id))
			s.append(pokestring("\n        *")).append(pokestring(amt));
		menu->GetItems().push_back(new TextItem(menu, select, s, i, items[i].id));
	}
	menu->GetItems().push_back(new TextItem(menu, [this](TextItem* src){ this->GetMenu()->Close(); }, pokestring("CANCEL")));
	menu->UpdateMenu();
}

bool ItemStorage::AddItem(unsigned char id, unsigned char quantity)
{
	unsigned char room_for = 0;
	for (int i = 0; i < MAX_ITEMS && room_for < quantity && i < items.size(); i++)
	{
		if (items[i].Empty())
			room_for += 99;
		else if (items[i].id == id)
			room_for += 99 - items[i].quantity;
	}
	if (room_for < quantity)
		return false;

	for (int i = 0; i < MAX_ITEMS && i < items.size(); i++)
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

void ItemStorage::RemoveItem(unsigned char id, unsigned char quantity)
{
	for (int i = items.size() - 1; i >= 0 && quantity > 0; i--)
	{
		if (items[i].id != id)
			continue;
		unsigned char amount = min(quantity, items[i].quantity);
		items[i].quantity -= amount;
		quantity -= amount;
		if (items[i].quantity == 0)
		{
			items[i].id = 0;
			items.erase(items.begin());
			items.push_back(Item());
		}
	}

	GenerateItems();
}

void ItemStorage::RemoveItemFromSlot(unsigned char slot, unsigned char quantity)
{
	if (slot >= MAX_ITEMS || slot >= items.size())
		return;
	quantity = min(items[slot].quantity, quantity);
	items[slot].quantity -= quantity;
	if (items[slot].quantity == 0)
	{
		items[slot].id = 0;
		for (int i = slot; i < MAX_ITEMS - 1 && i < items.size() - 1; i++)
		{
			items[i] = items[i + 1];
		}
		items[MAX_ITEMS - 1].id = 0;
		items[MAX_ITEMS - 1].quantity = 0;
	}

	GenerateItems();
}

unsigned char ItemStorage::GetItemCount()
{
	for (unsigned char i = 0; i < MAX_ITEMS && i < items.size(); i++)
	{
		if (items[i].id == 0 || items[i].quantity == 0)
			return i;
	}
	return items.size();
}