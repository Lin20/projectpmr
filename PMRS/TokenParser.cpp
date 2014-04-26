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
	return true;
}

bool TokenParser::IsVariable(string& token)
{
	if (token.find("\"") != string::npos)
		return false;
	return true;
}

bool TokenParser::IsString(string& token)
{
	return false;
}

string TokenParser::GetDirectory(string& filename)
{
	unsigned int found = filename.find_last_of("/\\");
	return filename.substr(0, found);
}

string TokenParser::ToLower(string s)
{
	transform(s.begin(), s.end(), s.begin(), ::tolower);
	return s;
}