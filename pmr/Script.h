#pragma once

#include <string>
#include "Utils.h"
#include "Variable.h"
#include "DataBlock.h"
#include "Common.h"
#include "ResourceCache.h"
#include "OverworldEntity.h"

using namespace std;

#define MAX_VARS	1024

class Script
{
public:
	Script(MapScene* on_scene);
	Script(MapScene* on_scene, const string& filename);
	~Script();

	static Script* TryLoad(MapScene* on_scene, unsigned char map, unsigned char script_index);
	static bool CheckExists(unsigned char map, unsigned char script_index);

	bool Load(const string& filename);
	bool Load(unsigned char map, unsigned char script_index);
	bool Done();
	void Reset();
	void Update();

	inline Textbox* GetBuiltMenu() { return built_menu; }
	inline void SetMenuResult(unsigned char index) { variables[menu_variable ].int_value = index; }
	inline void ClearBuiltMenu() { built_menu = 0; }
	inline void SetEntityIndex(unsigned char index) { entity_index = index; }

private:
	MapScene* on_scene;

	DataBlock* buffer;
	Variable variables[MAX_VARS];
	vector<unsigned char> watch_entities; //stop if any exist that aren't snapped
	Textbox* built_menu;
	unsigned int menu_variable;
	unsigned char menu_result;
	bool entity_wait;
	unsigned int delay;
	unsigned char entity_index;

	void ResetVariables();
	bool CheckSpace(unsigned char amount);
	void SetVariable(unsigned int index);
	Variable GetVariable();
	string ReadString();
};