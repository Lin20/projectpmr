#include "ItemStorage.h"

ItemStorage::ItemStorage()
{
	for (int i = 0; i < MAX_ITEMS; i++)
		items.push_back(Item((i < 15 ? i + 1 : 0), rand() % 20 + 5));

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
	auto select = [this](TextItem* item)
	{
		auto toss = [this](TextItem* item)
		{
			if (ResourceCache::IsKeyItem(item->value))
			{
				Textbox* t = new Textbox();
				t->SetText(new TextItem(t, nullptr, pokestring("That's too impor-\ntant to toss!\f")));
			}
			else
			{
				auto confirm = [this]()
				{
					auto confirm_toss_b = [this](TextItem* from)
					{
						from->GetParent()->CancelClose();
						Textbox* yesno = new Textbox(14, 7, 6, 5);
						yesno->SetMenu(true, 2, sf::Vector2i(1, 0), sf::Vector2u(0, 2), nullptr, MenuFlags::FOCUSABLE);
						yesno->SetArrowState(ArrowStates::ACTIVE);
						yesno->GetItems().push_back(new TextItem(yesno, nullptr, pokestring("YES")));
						yesno->GetItems().push_back(new TextItem(yesno, nullptr, pokestring("NO")));
						yesno->UpdateMenu();
						from->GetParent()->ShowTextbox(yesno);
					};
					Textbox* tb = new Textbox();
					tb->SetText(new TextItem(tb, confirm_toss_b, pokestring("Is it OK to toss\n").append(ResourceCache::GetItemName(this->GetItems()[this->GetMenu()->GetInactiveIndex()].id)).append(pokestring("?\f"))));
					this->GetMenu()->ShowTextbox(tb);
				};
				Textbox* t = new Textbox(15, 9, 5, 3);
				t->SetCounter(true, 1, this->GetItems()[item->index].quantity, confirm, [this]()
				{
					this->GetMenu()->GetTextboxes()[0]->SetArrowState(ArrowStates::ACTIVE);
				});
				this->GetMenu()->ShowTextbox(t);
				this->GetMenu()->GetTextboxes()[0]->SetArrowState(ArrowStates::INACTIVE);
			}
		};

		auto use = [this](TextItem* item)
		{
			Textbox* t = new Textbox();
			t->SetText(new TextItem(t, [this](TextItem* src){ this->GetMenu()->GetTextboxes()[0]->SetArrowState(ArrowStates::ACTIVE); }, pokestring("You just used\n").append(ResourceCache::GetItemName(item->value)).append(pokestring("."))));
			this->GetMenu()->ShowTextbox(t);
			this->GetMenu()->GetTextboxes()[0]->SetArrowState(ArrowStates::INACTIVE);
		};

		this->GetMenu()->SetArrowState(ArrowStates::INACTIVE);
		
		Textbox* usetoss = new Textbox(13, 10, 7, 5);
		usetoss->SetMenu(true, 2, sf::Vector2i(1, 0), sf::Vector2u(0, 2), [this](){this->GetMenu()->SetArrowState(ArrowStates::ACTIVE); }, MenuFlags::FOCUSABLE);
		usetoss->GetItems().push_back(new TextItem(usetoss, use, pokestring("USE"), this->GetMenu()->GetInactiveIndex(), this->GetItems()[this->GetMenu()->GetInactiveIndex()].id));
		
		usetoss->GetItems().push_back(new TextItem(usetoss, toss, pokestring("TOSS"), this->GetMenu()->GetInactiveIndex(), this->GetItems()[this->GetMenu()->GetInactiveIndex()].id));
		usetoss->UpdateMenu();
		usetoss->SetArrowState(ArrowStates::ACTIVE);
		menu->ShowTextbox(usetoss);
	};

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
		menu->GetItems().push_back(new TextItem(menu, select, s, i, items[i].id));
	}
	menu->GetItems().push_back(new TextItem(menu, [this](TextItem* src){ this->GetMenu()->Close(); }, pokestring("CANCEL")));
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