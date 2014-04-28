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
	if (!((token[0] >= 'A' && token[0] <= 'Z') || (token[0] >= 'a' && token[0] <= 'z') || token[0] == '_'))
		return false;
	for (unsigned int i = 1; i < token.length(); i++)
	{
		if (!((token[i] >= 'A' && token[i] <= 'Z') || (token[i] >= 'a' && token[i] <= 'z') || (token[i] >= '0' && token[i] <= '9') || token[i] == '_'))
			return false;
	}
	return true;
}

bool TokenParser::IsLabel(string& token, string& destination)
{
	if (token[token.length() - 1] != ':')
		return false;
	string s = token.substr(0, token.length() - 1);
	if (IsVariable(s))
	{
		destination = s;
		return true;
	}
	return false;
}

bool TokenParser::IsString(string& token)
{
	return false;
}

bool TokenParser::IsDirective(string& token)
{
	if (token[0] != '#')
		return false;
	return true;
}

string TokenParser::GetDirectory(string& filename)
{
	unsigned int found = filename.find_last_of("/\\");
	return filename.substr(0, found);
}

string TokenParser::GetFilename(string& filename)
{
	unsigned int found = filename.find_last_of("/\\");
	string file = filename.substr(found + 1);
	unsigned int dot = file.find_last_of(".");
	if (dot != string::npos)
		return file.substr(0, dot);
	return file;
}

string TokenParser::ToLower(string s)
{
	transform(s.begin(), s.end(), s.begin(), ::tolower);
	return s;
}

bool TokenParser::IsInteger(string& token, unsigned int& out)
{
	int base = 10;
	string n = token;
	if (token[0] == '$') //hexadecimal $ab123
	{
		base = 16;
		n = n.substr(1);
	}
	else if (token[0] == '%') //binary %10110110
	{
		base = 2;
		n = n.substr(1);
	}
	else if (token.length() > 2 && token[0] == '0' && token[1] == 'x') //hexadecimal 0xab123
	{
		base = 16;
		n = n.substr(2);
	}
	else if (token[0] < '0' || token[0] > '9')
		return false;
	if (n.length() == 0)
		return false;
	try
	{
		out = stoi(n, nullptr, base);
		return true;
	}
	catch (int e)
	{
		return false;
	}
}