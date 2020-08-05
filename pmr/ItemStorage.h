#pragma once

#include <vector>
#include "Constants.h"
#include "Common.h"
#include "Events.h"
#include "Textbox.h"
#include "MenuCache.h"
#include "ItemActions.h"

using namespace std;

class ItemStorage
{
public:
	ItemStorage(PlayerProperties* owner);
	~ItemStorage();

	PlayerProperties* GetOwner() { return owner; }
	vector<Item>& GetItems() { return items; }
	Textbox* GetMenu() { return menu; }
	bool AddItem(unsigned char id, unsigned char quantity);
	bool AddItem(Item i) { AddItem(i.id, i.quantity); }

	unsigned char GetQuantity(unsigned char id);
	unsigned char GetItemCount();
	void RemoveItem(unsigned char id, unsigned char quantity);
	void RemoveItemFromSlot(unsigned char slot, unsigned char quantity);
	void RemoveItem(Item i) { RemoveItem(i.id, i.quantity); }

private:
	PlayerProperties* owner;

	vector<Item> items;
	Textbox* menu;

	void GenerateItems();
};

