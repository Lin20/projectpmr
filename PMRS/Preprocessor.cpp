#include "Preprocessor.h"

map<string, string> Preprocessor::defines;

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

void Preprocessor::ProcessDirectives(vector<Line>& lines, string& formatted_code, string filename)
{
	for (unsigned int i = 0; i < lines.size(); i++)
	{
		vector<string>& tokens = lines[i].GetTokens();
		unsigned int k = 1;
		if (tokens[0] == "#define")
			k = 2;
		for (; k < tokens.size(); k++) //don't replace the main command
		{
			if (TokenParser::IsVariable(tokens[k]))
			{
				map<string, string>::iterator it = defines.find(tokens[k]);
				if (it != defines.end())
				{
					lines[i].SetToken(k, it->second);
				}
			}
		}

		//lines will never be empty since they're formatted beforehand, so we don't need to check for them
		if (lines[i].GetFormattedText()[0] != '#')
			continue;

		//no switch(string) :(
		//replace the defines inside it
		string token = tokens[0];
		if (token == "#include")
		{
			IncludeFile(lines, i, formatted_code, tokens, filename);
		}
		else if (token == "#define")
		{
			AddDefinition(lines[i]);
		}
	}
}

void Preprocessor::IncludeFile(vector<Line>& lines, unsigned int& line_index, string& formatted_code, vector<string>& tokens, string this_filename)
{
	string filename;
	Line& line = lines[line_index];
	if (tokens.size() > 1)
	{
		if (TokenParser::IsPureString(line.GetTokens()[1], filename))
		{
			if (tokens.size() > 2)
			{
				ErrorReporter::AddWarning("Ignored parameters (expects 2).", line.GetLineNumber(), line.GetFormattedText());
			}

			//now actually include the file
			filename = TokenParser::GetDirectory(this_filename).append("/").append(filename);
			ifstream file(filename);
			if (!file)
			{
				ErrorReporter::AddError(string("Cannot find file \'").append(filename).append("\'."), line.GetLineNumber(), line.GetFormattedText());
			}
			else
			{
				std::string include_src((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
				vector<Line> include_lines;
				Preprocessor::Process(include_src);
				Preprocessor::FindLines(include_lines, include_src);
				Preprocessor::ProcessDirectives(include_lines, include_src, filename);
				for (unsigned int i = 0; i < include_lines.size(); i++)
				{
					lines.insert(lines.begin() + line_index + i, include_lines[i]);
				}
				line_index += include_lines.size();
			}
		}
		else
		{
			ErrorReporter::AddError(string("\'").append(tokens[1]).append("\' is not a pure string literal."), line.GetLineNumber(), line.GetFormattedText());
		}
	}
	else
	{
		ErrorReporter::AddError("Expected pure string literal.", line.GetLineNumber(), line.GetFormattedText());
	}
}

void Preprocessor::AddDefinition(Line& line)
{
	vector<string>& tokens = line.GetTokens();
	if (tokens.size() > 2)
	{
		string name = tokens[1];
		if (defines.find(name) != defines.end())
		{
			ErrorReporter::AddError(string("Redefinition of ").append(tokens[1]).append("."), line.GetLineNumber(), line.GetFormattedText());
		}
		else
		{
			defines.insert(pair<string, string>(name, line.GetFormattedText().substr(line.GetFormattedText().find(",", line.GetFormattedText().find(",") + 1) + 1)));
		}
	}
	else
	{
		ErrorReporter::AddWarning("Empty define statement.", line.GetLineNumber(), line.GetFormattedText());
	}
}