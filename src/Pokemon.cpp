#include "Pokemon.h"


Pokemon::Pokemon(unsigned char index, unsigned char level)
{
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