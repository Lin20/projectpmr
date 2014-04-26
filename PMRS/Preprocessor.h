#pragma once

#include <string>
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
	static void ProcessDirectives(vector<Line>& lines, string& formatted_code);

private:
	static void RemoveComments(string& code);
	static void RemoveWhitespace(string& code);
};