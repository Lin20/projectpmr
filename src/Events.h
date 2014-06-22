#pragma once
#include "DataBlock.h"

struct Warp
{
	unsigned char x;
	unsigned char y;
	unsigned char dest_point;
	unsigned char dest_map;
	unsigned char type; //used to determine what kind of warp it is based on Map::CanWarp (used for determining whether or not the player walks forward after exiting a map)

	Warp(unsigned char map = 0)
	{
		x = 0;
		y = 0;
		dest_point = 0;
		dest_map = map;
		type = 0;
	}
};

struct Sign
{
	unsigned char x;
	unsigned char y;
	unsigned char text;
};

struct Entity
{
	unsigned char x;
	unsigned char y;
	unsigned char sprite;
	unsigned char movement1;
	unsigned char movement2;
	unsigned char text;
	unsigned char trainer;
	unsigned char pokemon_set;
	unsigned char item;
};

struct Item
{
	unsigned char id;
	unsigned char quantity;

	Item()
	{
		id = 0;
		quantity = 0;
	}

	Item(unsigned char id, unsigned char quantity)
	{
		this->id = id;
		this->quantity = quantity;
	}

	bool Empty() { return id == 0 || quantity == 0; }
};

struct Evolution
{
	unsigned char trigger;
	unsigned char item;
	unsigned char level;
	unsigned char pokemon;

	Evolution(unsigned char _trigger = 0, unsigned char _item = 0, unsigned char _level = 0, unsigned char _pokemon = 0)
	{
		trigger = _trigger;
		item = _item;
		level = _level;
		pokemon = _pokemon;
	}

	void Load(DataBlock* d)
	{
		trigger = d->getc();
		if (!trigger)
			return;
		switch (trigger)
		{
		case 2:
			item = d->getc();
		case 1:
		case 3:
			level = d->getc();
			pokemon = d->getc();
			break;
		}
	}
};

struct LearnsetMove
{
	unsigned char level;
	unsigned char move;

	LearnsetMove(unsigned char _level = 0, unsigned char _move = 0)
	{
		level = _level;
		move = _move;
	}

	void Load(DataBlock* d)
	{
		level = d->getc();
		if (!level)
			return;
		move = d->getc();
	}
};

struct FlyPoint
{
	unsigned char map;
	unsigned char x;
	unsigned char y;

	FlyPoint(unsigned char _map = 0, unsigned char _x = 0, unsigned char _y = 0)
	{
		map = _map;
		x = _x;
		y = _y;
	}

	void Load(DataBlock* d)
	{
		map = d->getc();
		y = d->getc();
		x = d->getc();
	}
};

struct WildEncounter
{
	bool water;
	unsigned char id;
	unsigned char level;

	WildEncounter(bool _water = false, unsigned char _id = 0, unsigned char _level = 0)
	{
		water = _water;
		id = _id;
		level = _level;
	}
};
