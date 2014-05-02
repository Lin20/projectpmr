#include "NPC.h"


NPC::NPC(Map* on_map, Entity data, Script* script) : OverworldEntity(on_map, data.sprite, data.x, data.y, ENTITY_DOWN, true, script)
{
	this->data = data;
	occupation = 0;
	PerformAI(true);
}

NPC::~NPC()
{
}

void NPC::Update()
{
	OverworldEntity::Update();
	PerformAI();
}

void NPC::PerformAI(bool force)
{
	if (frozen)
		return;
	if (script_enabled && script && !script->Done())
		return;
	if (occupation)
	{
		if (occupation->SetToClose())
			occupation = 0;
		return;
	}

	if ((force && data.movement1 != MTYPE_DIRECTIONAL) || !force)
	{
		if (rand() % 255 >= RANDOM_WALK || steps_remaining > 0)
			return;
	}

	switch (data.movement1)
	{
	case MTYPE_WALK:
		Move(GetNextDirection(), 1);
		break;

	case MTYPE_DIRECTIONAL:
		Face(GetNextDirection());
		break;
	}
}

unsigned char NPC::GetNextDirection()
{
	switch (data.movement2)
	{
	case MDIR_ANY:
	case MDIR_NONE:
		return rand() % 4;
		break;

	case MDIR_DOWN:
	case MDIR_UP:
	case MDIR_LEFT:
	case MDIR_RIGHT:
		return data.movement2 - 208;
		break;

	case MDIR_VERTICAL:
		return ENTITY_DOWN + rand() % 2;
		break;

	case MDIR_HORIZONTAL:
		return ENTITY_LEFT + rand() % 2;
		break;
	}

	return MDIR_NONE;
}