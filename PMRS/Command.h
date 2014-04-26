#pragma once

#include <string>
#include <vector>

using namespace std;

struct Command
{
	unsigned char opcode = 0;
	string name = "";
	string description = "";
	vector<string> params = vector<string>();
};