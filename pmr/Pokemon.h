#pragma once

#include "StringConverter.h"
#include "Move.h"
#include "Events.h"
#include <math.h>

//this is never going to be used without the inclusion of Pokemon.h
//so why not just declare it here
enum Types
{
	NORMAL = 0,
	FIGHTING = 1,
	FLYING = 2,
	POISON = 3,
	GROUND = 4,
	ROCK = 5,
	BUG = 7,
	GHOST = 8,
	FIRE = 0x14,
	WATER = 0x15,
	GRASS = 0x16,
	ELECTRIC = 0x17,
	PSYCHIC = 0x18,
	ICE = 0x19,
	DRAGON = 0x1A,
};

enum Statuses
{
	OK = 0,
	POISONED = 1,
	SLEEPING = 2,
	PARALYZED = 3,
	BURNED = 4,
	FROZEN = 5,
	FAINTED	= 6
};

class Pokemon
{
public:
	Pokemon(unsigned char index = 0, unsigned char level = 1);
	~Pokemon();

	//i really hate making accessors and mutators
	//so to avoid having 20 of them im just going to make the variables public.
	//sue me.

	unsigned char id;
	unsigned char pokedex_index;
	unsigned short ot;
	unsigned char level;
	unsigned int xp;
	unsigned char growth_rate;
	unsigned char catch_rate;
	unsigned char xp_yield;
	unsigned int hp;
	string ot_name;
	bool has_nickname;

	string original_name;
	string nickname;
	unsigned char type1;
	unsigned char type2;

	unsigned char base_hp;
	unsigned char base_attack;
	unsigned char base_defense;
	unsigned char base_speed;
	unsigned char base_special;

	unsigned short max_hp;
	unsigned short attack;
	unsigned short defense;
	unsigned short speed;
	unsigned short special;

	unsigned short ev_hp;
	unsigned short ev_attack;
	unsigned short ev_defense;
	unsigned short ev_speed;
	unsigned short ev_special;

	unsigned char dv_hp;
	unsigned char dv_attack;
	unsigned char dv_defense;
	unsigned char dv_speed;
	unsigned char dv_special;
	
	unsigned char size_x;
	unsigned char size_y;

	Move moves[4];
	Evolution evolutions[5];
	LearnsetMove learnset[16];

	unsigned char status;

	void LoadStats(bool default_moves = false, unsigned char* move_count = 0);
	void RecalculateStats();
	void Heal();

	unsigned int GetXPRemaining() { return GetXPAt(level + 1, growth_rate) - xp; }

	static unsigned int CalculateStat(unsigned char base, unsigned char dv, unsigned int xp, unsigned char level);

	static float CalculateStatXP(unsigned int xp, unsigned char level);
	static const char* GetTypeName(unsigned char type);
	static const char* GetStatusName(unsigned char s);
	static unsigned int GetXPAt(unsigned char level, unsigned char exp_type);

private:
};
