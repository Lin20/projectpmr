#include "Preprocessor.h"


Preprocessor::Preprocessor()
{
}

Preprocessor::~Preprocessor()
{
}

void Preprocessor::Process(string& code)
{
	RemoveComments(code);
	RemoveWhitespace(code);
}

void Preprocessor::RemoveComments(string& code)
{
	bool in_comment = false;
	bool in_string = false;
	for (unsigned int i = 0; i < code.length(); i++)
	{
		char at = code[i];
		if (at == ';' && !in_string)
		{
			in_comment = !in_comment;
			code[i] = ' ';
		}
		else if (at == '\n')
		{
			in_comment = in_string = false;
		}
		else if (in_comment)
		{
			code[i] = ' ';
		}
		else if (at == '\"')
		{
			in_string = !in_string;
		}
		else if (at == '\\')
		{
			i++;
		}
	}
}

void Preprocessor::RemoveWhitespace(string& code)
{
	bool in_comment = false;
	bool in_string = false;
	char previous = ' ';
	bool first_space = true; //this stupid flag makes it so the first word is a token separated by spaces, whereas the rest of the tokens are split using commas
	//this is so we can do something like '#include "hi.txt"'  instead of '#include: "hi.txt"'
	for (unsigned int i = 0; i < code.length(); i++)
	{
		if (i == code.length() - 1)
		{
			code[i] = code[i];
		}
		char at = code[i];
		if (at == '\"')
		{
			in_string = !in_string;
		}
		else if (at == '\\')
		{
			i++;
		}
		else if (at == '\n')
		{
			in_comment = in_string = false;
			first_space = true;
			if (previous == ' ')
			{
				code.erase(code.begin() + i - 1);
				i--;
			}
		}
		else if (!in_string && ((at == ' ' && (previous == ' ' || previous == ',' || previous == '+' || i == code.length() - 1)) || at == '\r'))
		{
			code.erase(code.begin() + i);
			i--;
		}
		else if (at == ' ' && !in_string && first_space)
		{
			code[i] = ',';
			at = ',';
			first_space = false;
		}
		else if (i > 0 && previous == ' ' && at == '+')
		{
			code.erase(code.begin() + i - 1);
			i--;
		}
		previous = at;
	}
	if (code.length() > 0 && code[code.length() - 1] == ' ')
		code.erase(code.length() - 1);
}

void Preprocessor::FindLines(vector<Line>& lines, string& formatted_code)
{
	unsigned int at = 0;
	unsigned int line_count = 0;
	while (at < formatted_code.length())
	{
		char c = formatted_code[at];
		if (c == '\n')
		{
			line_count++;
			continue;
		}
		lines.push_back(Line(formatted_code, at, line_count++));
	}
}

void Preprocessor::ProcessDirectives(vector<Line>& lines, string& formatted_code)
{
	for (unsigned int i = 0; i < lines.size(); i++)
	{
		//lines will never be empty since they're formatted beforehand, so we don't need to check for them
		if (lines[i].GetFormattedText()[0] != '#')
			continue;

		//no switch(string) :(
		vector<string>& tokens = lines[i].GetTokens();
		string token = tokens[0];
		if (token == "#include")
		{
			if (tokens.size() > 1)
			{
				string filename;
				if (TokenParser::IsPureString(lines[i].GetTokens()[1], filename))
				{
					if (tokens.size() > 2)
					{
						ErrorReporter::AddWarning("Ignored parameters (expects 2).", lines[i].GetLineNumber(), lines[i].GetFormattedText());
					}
				}
				else
				{
					ErrorReporter::AddError(string("\'").append(tokens[1]).append("\' is not a pure string literal."), lines[i].GetLineNumber(), lines[i].GetFormattedText());
				}
			}
			else
			{
				ErrorReporter::AddError("Expected pure string literal.", lines[i].GetLineNumber(), lines[i].GetFormattedText());
			}
		}
	}
}