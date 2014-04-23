#pragma once

#include <vector>
#include "Constants.h"
#include "Common.h"
#include "Events.h"
#include "Textbox.h"
#include "MenuCache.h"

using namespace std;

class ItemStorage
{
public:
	ItemStorage();
	~ItemStorage();

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
	vector<Item> items;
	Textbox* menu;

	void GenerateItems();
};

