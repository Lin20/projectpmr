#include "Pokemon.h"


Pokemon::Pokemon(unsigned char index, unsigned char l)
{
 	id = index;
	level = l;
	pokedex_index = ResourceCache::GetPokedexIndex(index);
	ot = 0;
	xp = 0;
	original_name = ResourceCache::GetPokemonName(index);
	nickname = original_name;
	type1 = 0;
	type2 = 0;

	DataBlock* data = ResourceCache::GetPokemonStats(index);
	if (data)
	{
		base_hp = data->getc();
		base_attack = data->getc();
		base_defense = data->getc();
		base_speed = data->getc();
		base_special = data->getc();

		type1 = data->getc();
		type2 = data->getc();

		catch_rate = data->getc();
		xp_yield = data->getc();

		//moves
		data->getc(); data->getc(); data->getc(); data->getc();
		growth_rate = data->getc();
	}
	ev_hp = 0;
	ev_attack = 0;
	ev_defense = 0;
	ev_speed = 0;
	ev_special = 0;

	dv_hp = 0;
	dv_attack = 0;
	dv_defense = 0;
	dv_speed = 0;
	dv_special = 0;

	RecalculateStats();
}

Pokemon::~Pokemon()
{
}

void Pokemon::RecalculateStats()
{
	max_hp = (2 * base_hp + dv_hp + ev_hp / 4) * level / 100 + 10 + level;
	attack = CalculateStat(base_attack, dv_attack, ev_attack, level);
	defense = CalculateStat(base_defense, dv_defense, ev_defense, level);
	speed = CalculateStat(base_speed, dv_speed, ev_speed, level);
	special = CalculateStat(base_special, dv_special, ev_special, level);
}

unsigned int Pokemon::CalculateStat(unsigned char base, unsigned char dv, unsigned int ev, unsigned char level)
{
	return (2 * base + dv + ev / 6) * level / 100 + 5;
}