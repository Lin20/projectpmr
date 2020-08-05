#pragma once

#include <string>
#include "Line.h"

using namespace std;

struct Label
{
	unsigned int offset;
	string name;
	Line* line;

	Label(unsigned int off, string n, Line* l)
	{
		offset = off;
		name = n;
		line = l;
	}
};