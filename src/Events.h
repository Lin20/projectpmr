#pragma once

struct Warp
{
	unsigned char x;
	unsigned char y;
	unsigned char dest_point;
	unsigned char dest_map;
	unsigned char type; //used to determine what kind of warp it is based on Map::CanWarp (used for determining whether or not the player walks forward after exiting a map)
};

struct Entity
{
	unsigned char x;
	unsigned char y;
	unsigned char sprite;
	unsigned char movement_1;
	unsigned char movement_2;
	unsigned char text;
	unsigned char trainer;
	unsigned char pokemon_set;
	unsigned char item;
};