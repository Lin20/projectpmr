#pragma once

#include "Common.h"
#include "ItemStorage.h"
#include "OverworldEntity.h"
#include "Options.h"
#include "Pokemon.h"

class PlayerProperties
{
public:
	PlayerProperties()
	{
		pokemon_count = 6;
		//generate some pokemon for testing
		for (int i = 0; i < 6; i++)
		{
			party[i] = Pokemon(rand() % 190, rand() % 99 + 2);
		}
	}
	~PlayerProperties()
	{
		//we can't delete the inventory here because of circular inclusion
	}

	ItemStorage* GetInventory() { return inventory; }
	OverworldEntity* GetCorrespondingEntity() { return corresponding_entity; }
	Options& GetOptions(){ return options; }
	Pokemon* GetParty() { return party; }

	void SetInventory(ItemStorage* i) { inventory = i; }

private:
	ItemStorage* inventory;
	OverworldEntity* corresponding_entity;
	Options options;
	Pokemon party[6];
	unsigned char pokemon_count;
};