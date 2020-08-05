#pragma once
#include "Common.h"
#include "OverworldEntity.h"
#include "Events.h"
#include "Textbox.h"

class NPC : public OverworldEntity
{
public:
	NPC(Map* on_map, unsigned char index, Entity data, Script* script = 0, std::function<void()> step_callback = nullptr);
	~NPC();

	void Update() override;
	Textbox* occupation;

private:
	Entity data;

	void PerformAI(bool force = false);
	unsigned char GetNextDirection();
};