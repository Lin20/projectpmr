#pragma once

#include <string>

using namespace std;

class TokenParser
{
public:
	TokenParser();
	~TokenParser();

	static bool IsPureString(string& token, string& destination);
	static bool IsString(string& token);

private:
};

