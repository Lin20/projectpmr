#pragma once

#include <string>
#include <vector>
#include "ErrorReporter.h"

using namespace std;

class Line
{
public:
	Line(string& code, unsigned int& start, unsigned int line);
	~Line();

	inline vector<string>& GetTokens() { return tokens; }
	inline string& GetFormattedText() { return formatted_text; }
	inline unsigned int GetLineNumber() { return line_number; }

	void SetToken(unsigned int token_index, string value);

private:
	string formatted_text;
	vector<string> tokens;
	unsigned int line_number;

	void FindTokens(string& code, unsigned int& start);
};