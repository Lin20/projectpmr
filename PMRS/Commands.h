#pragma once

/*
 * Man we use so many C functions here
 * I almost feel like my chest should sprout a few pieces of hair
 */

#include <stdlib.h>
#include <cstring>
#include <sstream>
#include <string>
#include <fstream>
#include "Command.h"

#define COMMANDS_LOCATION	"C:/scripting/commands.txt"

class Commands
{
public:
	Commands();
	~Commands();

	static void LoadCommands();

private:
	static Command commands[256];

	static void ProcessLine(string line);
};