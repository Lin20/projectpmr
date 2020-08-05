#pragma once

#include <string>
#include <map>
#include <fstream>
#include "Line.h"
#include "TokenParser.h"

using namespace std;

class Preprocessor
{
public:
	Preprocessor();
	~Preprocessor();

	static void Process(string& raw_code);
	static void FindLines(vector<Line>& lines, string& formatted_code);
	static void ProcessDirectives(vector<Line>& lines, string& formatted_code, string filename);

private:
	static void RemoveComments(string& code);
	static void RemoveWhitespace(string& code);

	static void IncludeFile(vector<Line>& lines, unsigned int& line_index, string& formatted_code, vector<string>& tokens, string filename);
	static void AddDefinition(Line& line);

	static map<string, string> defines;
};