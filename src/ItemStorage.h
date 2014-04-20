#pragma once

#include <vector>
#include "Constants.h"
#include "Common.h"
#include "Events.h"
#include "Textbox.h"

using namespace std;

class ItemStorage
{
public:
	ItemStorage();
	~ItemStorage();

	vector<Item>& GetItems() { return items; }
	bool AddItem(Item i);
	bool AddItem(unsigned char id, unsigned char quantity);

	unsigned char GetQuantity(unsigned char id);
	void RemoveItem(Item i);
	void RemoveItem(unsigned char id, unsigned char quantity);
	void RemoveItem(unsigned char slot);

private:
	vector<Item> items;
	Textbox* menu;

	void GenerateItems();
};

