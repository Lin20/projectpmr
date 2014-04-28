#pragma once
#include "Common.h"
#include "OverworldEntity.h"
#include "Events.h"
#include "Textbox.h"

class NPC : public OverworldEntity
{
public:
	NPC(Map* on_map, Entity data, Script* script = 0);
	~NPC();

	void Update() override;
	Textbox* occupation;

private:
	Entity data;

	void PerformAI(bool force = false);
	unsigned char GetNextDirection();
};