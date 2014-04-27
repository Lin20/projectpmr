#pragma once

#include <string>
#include "Line.h"

using namespace std;

struct Label
{
	unsigned char offset;
	string name;
	Line* line;

	Label(unsigned char off, string n, Line* l)
	{
		offset = off;
		name = n;
		line = l;
	}
};