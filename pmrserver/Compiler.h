#pragma once

#include <fstream>
#include <streambuf>
#include "Preprocessor.h"
#include "Commands.h"
#include "CommandParser.h"

class Compiler
{
public:
	Compiler(ostream& out, const char* filename, string output_dir, bool silent, bool debug_info);
	~Compiler();

	void Compile();

private:
	const char* filename;
	ostream& out;
	vector<Line> lines;
	bool silent;
	bool print_debug;
	string output_directory;
	unsigned char* output;
};