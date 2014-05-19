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
		for (int i = 0; i < 6; i++)
			party[i] = 0;
		name = pokestring("Lin");
		RandomParty();
	}
	~PlayerProperties()
	{
		//we can't delete the inventory here because of circular inclusion
		for (int i = 0; i < 6; i++)
			if (party[i])
				delete party[i];
	}

	ItemStorage* GetInventory() { return inventory; }
	OverworldEntity* GetCorrespondingEntity() { return corresponding_entity; }
	Options& GetOptions(){ return options; }
	Pokemon** GetParty() { return party; }
	string& GetName() { return name; }
	void RandomParty()
	{
		pokemon_count = 6;
		//generate some pokemon for testing
		for (int i = 4; i < 6; i++)
		{
			if (party[i])
				delete party[i];
			int ind = rand() % 190;
			while (ResourceCache::GetPokedexIndex(ind - 1) > 151)
				ind = rand() % 190;
			party[i] = new Pokemon(ind, rand() % 98 + 2);
		}
		party[0] = new Pokemon(0x47, 24);
		party[1] = new Pokemon(0x47, 24);
		party[2] = new Pokemon(0x47, 25);
		party[3] = new Pokemon(0x24, 17);

		party[0]->moves[2] = Move(20);
		party[0]->moves[3] = Move(21);
	}

	void SetInventory(ItemStorage* i) { inventory = i; }

private:
	string name;
	ItemStorage* inventory;
	OverworldEntity* corresponding_entity;
	Options options;
	Pokemon* party[6];
	unsigned char pokemon_count;
};