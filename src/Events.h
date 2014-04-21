#pragma once

struct Warp
{
	unsigned char x;
	unsigned char y;
	unsigned char dest_point;
	unsigned char dest_map;
	unsigned char type; //used to determine what kind of warp it is based on Map::CanWarp (used for determining whether or not the player walks forward after exiting a map)
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

	struct Item()
	{
		id = 0;
		quantity = 0;
	}

	struct Item(unsigned char id, unsigned char quantity)
	{
		this->id = id;
		this->quantity = quantity;
	}

	bool Empty() { return id == 0 || quantity == 0; }
};