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
	static bool IsString(string& token);

	static string GetDirectory(string& filename); //gets the directory without the trailing slash
	static string ToLower(string s);

private:
};

