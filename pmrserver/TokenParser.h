#pragma once

#include <string>
#include <algorithm>

using namespace std;

class TokenParser
{
public:
	TokenParser();
	~TokenParser();

	static bool IsPureString(string& token, string& destination);
	static bool IsVariable(string& token);
	static bool IsLabel(string& token, string& destination);
	static bool IsString(string& token);
	static bool IsDirective(string& token);

	static bool IsInteger(string& token, unsigned int& out);

	static string GetDirectory(string& filename); //gets the directory without the trailing slash
	static string GetFilename(string& filename);
	static string ToLower(string s);

private:
};

