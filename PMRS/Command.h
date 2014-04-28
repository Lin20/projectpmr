#pragma once

#include <string>
#include <vector>

using namespace std;

struct Command
{
	unsigned char opcode;
	string name;
	string description;
	unsigned char args_count;
	vector<string> params;
	Command()
	{
		opcode = 0;
		name = "";
		description = "";
		args_count = 0;
		params.clear();
	}
};