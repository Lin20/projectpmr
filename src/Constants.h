#pragma once

enum States
{
	OVERWORLD,
	OVERWORLD_MENU
};

//how the menu functions
enum MenuFlags
{
	NONE =			0,
	FOCUSABLE =		1,
	WRAPS =			2,
	HOLD_INPUT =	4 //whether or not a button can be held to cycle through items
};

//the state the cursor is in (flags because of moving items with both visible)
enum ArrowStates
{
	HIDDEN =	0,
	ACTIVE =	1,
	INACTIVE =	2
};

#define STARTING_MAP	1
#define STARTING_X		20
#define STARTING_Y		20

#define VIEWPORT_WIDTH	10
#define VIEWPORT_HEIGHT	9

#define CONNECTION_NORTH	0
#define CONNECTION_SOUTH	1
#define CONNECTION_WEST		2
#define CONNECTION_EAST		3
#define OUTSIDE_MAP			36

//warp stuff
#define WARP_TILE			0
#define WARP_TO_OUTSIDE		1
#define ELEVATOR_MAP		237

#define WATER_TILE 20
#define FLOWER_TILE 3
#define ANIMATION_TIMER 22

#define ENTITY_LIMIT		60
#define ENTITY_WALKSTART	12
#define ENTITY_DOWN			0
#define ENTITY_UP			1
#define ENTITY_LEFT			2
#define ENTITY_RIGHT		3
#define ISNPC(n) (n <= ENTITY_LIMIT)
#define STEP_TIMER			64.0f / 60.0f
#define DELTAX(n) (n == ENTITY_LEFT ? -1 : n == ENTITY_RIGHT ? 1 : 0)
#define DELTAY(n) (n == ENTITY_UP ? -1 : n == ENTITY_DOWN ? 1 : 0)
#define MOVEMENT_NORMAL		0
#define MOVEMENT_WALL		1
#define MOVEMENT_JUMP		2
#define MOVEMENT_NONE		255
#define JUMP_STEPS			16

//movement types
#define MTYPE_DIRECTIONAL	255
#define MTYPE_WALK			254
#define RANDOM_WALK			3
#define MDIR_ANY			0
#define MDIR_NONE			255
#define MDIR_DOWN			208
#define MDIR_UP				209
#define MDIR_LEFT			210
#define MDIR_RIGHT			211
#define MDIR_VERTICAL		1
#define MDIR_HORIZONTAL		2

#define MENU_BLANK			31
#define MENU_CORNER_UL		25
#define MENU_H				26
#define MENU_CORNER_UR		27
#define MENU_V				28
#define MENU_CORNER_DL		29
#define MENU_CORNER_DR		30
#define CURSOR_ACTIVE		109
#define CURSOR_INACTIVE 	108
#define CURSOR_MORE			110
#define CURSOR_MORE_TIME	45

#define MESSAGE_LINE		0x4F
#define MESSAGE_PARA		0x51
#define MESSAGE_END			0x57
#define MESSAGE_POKE		0x54
#define MESSAGE_PROMPT		0x58

//#define DEFAULT_PALETTE { sf::Color(255, 255, 255, 255), sf::Color(168, 168, 168, 255), sf::Color(96, 96, 96, 255), sf::Color(0, 0, 0, 255)}
#define DEFAULT_PALETTE_0 sf::Color(255, 255, 255, 255)
#define DEFAULT_PALETTE_1 sf::Color(168, 168, 168, 255)
#define DEFAULT_PALETTE_2 sf::Color(96, 96, 96, 255)
#define DEFAULT_PALETTE_3 sf::Color(0, 0, 0, 255)
#define PAL_0_RGBA	4177066239
#define PAL_1_RGBA	2829625599
#define PAL_2_RGBA	1616929023
#define PAL_3_RGBA	255

//temporary input
#define INPUT_UP		sf::Keyboard::Key::W
#define INPUT_DOWN		sf::Keyboard::Key::S
#define INPUT_LEFT		sf::Keyboard::Key::A
#define INPUT_RIGHT		sf::Keyboard::Key::D
#define INPUT_A			sf::Keyboard::Key::Z
#define INPUT_B			sf::Keyboard::Key::X
#define INPUT_START		sf::Keyboard::Key::Return
#define INPUT_SELECT	sf::Keyboard::Key::BackSpace
