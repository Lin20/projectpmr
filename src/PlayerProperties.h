#pragma once

#include "Common.h"
#include "ItemStorage.h"
#include "OverworldEntity.h"
#include "Options.h"

class PlayerProperties
{
public:
	PlayerProperties()
	{
	}
	~PlayerProperties()
	{
		//we can't delete the inventory here because of circular inclusion
	}

	ItemStorage* GetInventory() { return inventory; }
	OverworldEntity* GetCorrespondingEntity() { return corresponding_entity; }
	Options& GetOptions(){ return options; }

	void SetInventory(ItemStorage* i) { inventory = i; }

private:
	ItemStorage* inventory;
	OverworldEntity* corresponding_entity;
	Options options;
};