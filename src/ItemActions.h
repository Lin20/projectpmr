#pragma once

#include "Engine.h"
#include "Scene.h"
#include "Events.h"

class ItemActions
{
public:
	static void UseItem(ItemStorage* inventory, TextItem* src, unsigned char id);

	static void UseMedicine(TextItem* src);
	static void UseVitamin(TextItem* src);

private:
	//these are here because using an item on something like a pokemon requires a callback
	//with the only parameters being a TextItem*, so it needs some way of accessing the inventory.
	static ItemStorage* last_inventory;
	static TextItem* last_src;
	static unsigned char last_id;
};