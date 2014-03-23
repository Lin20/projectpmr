#pragma once

enum States
{
	OVERWORLD,
	OVERWORLD_MENU
};

#define STARTING_MAP	0
#define STARTING_X		5
#define STARTING_Y		5

#define VIEWPORT_WIDTH	20
#define VIEWPORT_HEIGHT	18

#define CONNECTION_NORTH	0
#define CONNECTION_SOUTH	1
#define CONNECTION_WEST		2
#define CONNECTION_EAST		3

#define WATER_TILE 20
#define FLOWER_TILE 3
#define ANIMATION_TIMER 22

#define ENTITY_LIMIT		60
#define ENTITY_WALKSTART	12
#define ENTITY_DOWN			0
#define ENTITY_UP			1
#define ENTITY_LEFT			2
#define ENTITY_RIGHT		3

//#define DEFAULT_PALETTE { sf::Color(255, 255, 255, 255), sf::Color(168, 168, 168, 255), sf::Color(96, 96, 96, 255), sf::Color(0, 0, 0, 255)}
#define DEFAULT_PALETTE_0 sf::Color(255, 255, 255, 255)
#define DEFAULT_PALETTE_1 sf::Color(168, 168, 168, 255)
#define DEFAULT_PALETTE_2 sf::Color(96, 96, 96, 255)
#define DEFAULT_PALETTE_3 sf::Color(0, 0, 0, 255)