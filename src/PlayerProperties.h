#pragma once

#include "Common.h"
#include "ItemStorage.h"
#include "OverworldEntity.h"

class PlayerProperties
{
public:
	PlayerProperties()
	{
	}
	~PlayerProperties()
	{
		if (inventory)
			delete inventory;
	}

	ItemStorage* GetInventory() { return inventory; }
	OverworldEntity* GetCorrespondingEntity() { return corresponding_entity; }

	void SetInventory(ItemStorage* i) { inventory = i; }

private:
	ItemStorage* inventory;
	OverworldEntity* corresponding_entity;
};