#include "TokenParser.h"


TokenParser::TokenParser()
{
}

TokenParser::~TokenParser()
{
}

bool TokenParser::IsPureString(string& token, string& destination)
{
	if (token.length() < 2 || token[0] != '\"' || token[token.length() - 1] != '\"')
		return false;

	for (unsigned int i = 1; i < token.length() - 1; i++)
	{
		if (token[i] == '\"')
			return false;
	}
	destination = token.substr(1, token.length() - 2);
}

bool TokenParser::IsString(string& token)
{
	return false;
}