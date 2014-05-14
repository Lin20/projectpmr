#include "Pokemon.h"


Pokemon::Pokemon(unsigned char index, unsigned char l)
{
	id = index;
	level = l;
	pokedex_index = ResourceCache::GetPokedexIndex(index - 1);
	ot = rand() % 100000;
	original_name = ResourceCache::GetPokemonName(index - 1);
	nickname = original_name;
	type1 = 0;
	type2 = 0;
	ot_name = pokestring("Lin");
	status = rand() % 5 + 1;
	unsigned char move_count = 0;

	DataBlock* data = ResourceCache::GetPokemonStats(pokedex_index - 1);
	if (data)
	{
		data->data = data->data_start;
		data->getc(); //pokedex index; we don't use it because according to padz it's a leftover value
		base_hp = data->getc();
		base_attack = data->getc();
		base_defense = data->getc();
		base_speed = data->getc();
		base_special = data->getc();

		type1 = data->getc();
		type2 = data->getc();

		catch_rate = data->getc();
		xp_yield = data->getc();

		unsigned char size = data->getc();
		size_x = size & 0xF;
		size_y = (size >> 4) & 0xF;

		//moves
		for (int i = 0; i < 4; i++)
		{
			moves[i] = Move(data->getc());
			if (moves[i].index)
				move_count++;
		}
		growth_rate = data->getc();
	}

	data = ResourceCache::GetPokemonLeveling(index - 1);
	if (data)
	{
		data->data = data->data_start;
		for (int i = 0; i < 5; i++)
		{
			evolutions[i].Load(data);
			if (evolutions[i].trigger == 0)
			{
				for (int k = i + 1; k < 5; k++)
					evolutions[i].trigger = 0;
				break;
			}
		}

		for (int i = 0; i < 16; i++)
		{
			learnset[i].Load(data);
			if (!learnset[i].level)
			{
				for (int k = i + 1; k < 16; k++)
					learnset[i].level = 0;
				break;
			}
		}
	}

	//determine the pokemon's moveset
	for (int i = 0; i < 16; i++)
	{
		if (learnset[i].level && learnset[i].level <= level)
		{
			if (move_count < 4)
			{
				moves[move_count++] = Move(learnset[i].move);
			}
			else
			{
				//this was causing problems unfortunately. probably related to padding
				//memcpy(moves, moves + 1, sizeof(Move)* 3);
				moves[0] = moves[1];
				moves[1] = moves[2];
				moves[2] = moves[3];
				moves[3] = Move(learnset[i].move);
			}
		}
	}

	xp = GetXPAt(l, growth_rate);
	ev_hp = 0;
	ev_attack = 0;
	ev_defense = 0;
	ev_speed = 0;
	ev_special = 0;

	dv_attack = rand() % 16;
	dv_defense = rand() % 16;
	dv_speed = rand() % 16;
	dv_special = rand() % 16;
	dv_hp = ((dv_attack & 1) << 3) | ((dv_defense & 1) << 2) | ((dv_speed & 1) << 1) | (dv_special & 1);

	RecalculateStats();
	int by = rand() % 10 + 1;
	hp = max_hp / 3 / (by)* (rand() % by + 1);
}

Pokemon::~Pokemon()
{
}

void Pokemon::RecalculateStats()
{
	max_hp = (unsigned int)((float)(base_hp + dv_hp + 50) * (float)level / 50.0f + 10.0f + (float)CalculateStatXP(ev_hp, level));
	attack = CalculateStat(base_attack, dv_attack, ev_attack, level);
	defense = CalculateStat(base_defense, dv_defense, ev_defense, level);
	speed = CalculateStat(base_speed, dv_speed, ev_speed, level);
	special = CalculateStat(base_special, dv_special, ev_special, level);
}

unsigned int Pokemon::CalculateStat(unsigned char base, unsigned char dv, unsigned int ev, unsigned char level)
{
	return (unsigned int)((float)(base + dv + 50) * (float)level / 50.0f + 5.0f + CalculateStatXP(ev, level));
}

float Pokemon::CalculateStatXP(unsigned int xp, unsigned char level)
{
	if (xp == 0)
		return 0;
	float f = (float)((sqrt((double)xp - 1.0) + 1.0) * (double)level / 400.0);
	return f;
}

const char* Pokemon::GetTypeName(unsigned char type)
{
	switch (type)
	{
	case 0x00:
		return "NORMAL";
		break;
	case 0x01:
		return "FIGHTING";
		break;
	case 0x02:
		return "FLYING";
		break;
	case 0x03:
		return "POISON";
		break;
	case 0x04:
		return "GROUND";
		break;
	case 0x05:
		return "ROCK";
		break;
	case 0x07:
		return "BUG";
		break;
	case 0x08:
		return "GHOST";
		break;
	case 0x14:
		return "FIRE";
		break;
	case 0x15:
		return "WATER";
		break;
	case 0x16:
		return "GRASS";
		break;
	case 0x17:
		return "ELECTRIC";
		break;
	case 0x18:
		return "PSYCHIC";
		break;
	case 0x19:
		return "ICE";
		break;
	case 0x1A:
		return "DRAGON";
		break;
	}
	return "UNKNOWN";
}

const char* Pokemon::GetStatusName(unsigned char s)
{
	switch (s)
	{
	case Statuses::OK:
		return "OK ";
	case Statuses::POISONED:
		return "PSN";
	case Statuses::SLEEPING:
		return "SLP";
	case Statuses::PARALYZED:
		return "PAR";
	case Statuses::BURNED:
		return "BRN";
	case Statuses::FROZEN:
		return "FRZ";
	}
	return "OK ";
}

unsigned int Pokemon::GetXPAt(unsigned char level, unsigned char exp_type)
{
	level = min(100u, (unsigned int)level);
	switch (exp_type)
	{
	default: //case 1 and 2 never actually occur...
		return level * level * level;

	case 3:
		return (unsigned int)(((1.2 * pow(level, 3u)) - (15.0 * pow(level, 2u)) + (100.0 * (double)level - 140.0)));

	case 4:
		return (unsigned int)(pow(level, 3u) * .8);

	case 5:
		return (unsigned int)(pow(level, 3u) * 1.25);
	}
}