#pragma once

#include <string>
#include "Utils.h"
#include "Variable.h"
#include "DataBlock.h"
#include "Common.h"
#include "ResourceCache.h"
#include "Opcodes.h"
#include "OverworldEntity.h"

using namespace std;

#define MAX_VARS	1024

class Script
{
public:
	Script(MapScene* on_scene);
	~Script();

	static Script* TryLoad(MapScene* on_scene, unsigned char map, unsigned char script_index);
	static bool CheckExists(unsigned char map, unsigned char script_index);

	bool Load(string& filename);
	bool Load(unsigned char map, unsigned char script_index);
	bool Done();
	void Reset();
	void Update();

private:
	MapScene* on_scene;

	DataBlock* buffer;
	Variable variables[MAX_VARS];

	void ResetVariables();
	bool CheckSpace(unsigned char amount);
	void SetVariable(unsigned int index);
	Variable GetVariable();
	string ReadString();
};