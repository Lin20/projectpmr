#include "CommandParser.h"

map<string, unsigned int> CommandParser::labels;
map<string, unsigned int> CommandParser::variables;
vector<Label> CommandParser::fillin_labels;

CommandParser::CommandParser()
{
}

CommandParser::~CommandParser()
{
}

void CommandParser::ProcessCommandStage1(unsigned char*& dest, Line& line, unsigned int& offset)
{
	vector<string>& tokens = line.GetTokens();
	string name = tokens[0];
	string label;
	line.SetCodeOffset(offset);
	if (TokenParser::IsLabel(name, label))
	{
		map<string, unsigned int>::iterator it = labels.find(label);
		if (it != labels.end())
		{
			ErrorReporter::AddError(string("Redefinition of label \'").append(label).append("\'."), line.GetLineNumber(), line.GetFormattedText());
		}
		else
		{
			labels.insert(pair<string, unsigned int>(label, offset));
		}
		return;
	}
	if (TokenParser::IsDirective(name))
	{
		return;
	}

	unsigned int command_index = 0;
	Command* command = Commands::GetCommand(name, command_index);
	if (!command)
	{
		ErrorReporter::AddError(string("Unknown command \'").append(name).append("\'."), line.GetLineNumber(), line.GetFormattedText());
		return;
	}
	if (command->args_count != tokens.size() - 1)
	{
		ErrorReporter::AddError(string("Wrong number of arguments. Expected ").append(to_string(command->args_count)).append(" but got ").append(to_string(tokens.size() - 1)).append("."), line.GetLineNumber(), line.GetFormattedText());
		return;
	}

	*dest++ = command->opcode;
	offset++;
	unsigned int token_index = 1;
	for (unsigned int i = 0; i < command->params.size(); i++)
	{
		if (ProcessToken(line, dest, (token_index < tokens.size() ? tokens[token_index] : ""), command->params[i], offset))
			token_index++;
	}
}

void CommandParser::ProcessCommandStage2(unsigned char*& dest)
{
	for (unsigned int i = 0; i < fillin_labels.size(); i++)
	{
		map<string, unsigned int>::iterator it = labels.find(fillin_labels[i].name);
		if (it == labels.end())
		{
			ErrorReporter::AddError(string("Cannot find label \'").append(fillin_labels[i].name).append("\'."), fillin_labels[i].line->GetLineNumber(), fillin_labels[i].line->GetFormattedText());
		}
		else
		{
			dest[fillin_labels[i].offset] = it->second & 0xFF;
			dest[fillin_labels[i].offset + 1] = (it->second >> 8) & 0xFF;
		}
	}
}

bool CommandParser::ProcessToken(Line& line, unsigned char*& dest, string& token, string& param, unsigned int& offset)
{
	unsigned int value;
	if (TokenParser::IsInteger(param, value))
	{
		*dest++ = 0; //0 means raw value
		*dest++ = value & 0xFF;
		*dest++ = (value >> 8) & 0xFF;
		offset += 3;
		return false;
	}
	else if (ParamIsNumber(param)) //#
	{
		ParseAsInteger(line, dest, token, param, offset, true);
	}
	else if (param == "s") //string
	{
		ParseAsString(line, dest, token, param, offset, true);
	}
	else if (param == "e") //either string or number
	{
		if (!ParseAsInteger(line, dest, token, param, offset, false) && !ParseAsString(line, dest, token, param, offset, false))
		{
			ErrorReporter::AddError(string("Expected int, string, or variable name but got \'").append(token).append("\'."), line.GetLineNumber(), line.GetFormattedText());
		}
	}
	else if (param == "l") //specify label
	{
		if (!TokenParser::IsVariable(token))
		{
			ErrorReporter::AddError(string("Cannot convert \'").append(token).append("\' to label."), line.GetLineNumber(), line.GetFormattedText());
			return true;
		}
		fillin_labels.push_back(Label(offset, token, &line));
		*dest++ = 0;
		*dest++ = 0;
		offset += 2;
	}
	else if (param == "d") //declare variable
	{
		if (!TokenParser::IsVariable(token))
		{
			ErrorReporter::AddError(string("Cannot convert \'").append(token).append("\' to variable name."), line.GetLineNumber(), line.GetFormattedText());
			return true;
		}
		map<string, unsigned int>::iterator it = variables.find(token);
		if (it != variables.end())
		{
			ErrorReporter::AddError(string("Redefinition of variable \'").append(token).append("\'."), line.GetLineNumber(), line.GetFormattedText());
		}
		else
		{
			unsigned int index = variables.size();
			variables.insert(pair<string, unsigned int>(token, index));
			*dest++ = (index & 0xFF);
			*dest++ = (index >> 8) & 0xFF;
			offset += 2;
		}
	}
	else if (param == "v") //specify variable
	{
		if (!TokenParser::IsVariable(token))
		{
			ErrorReporter::AddError(string("Cannot convert \'").append(token).append("\' to variable name."), line.GetLineNumber(), line.GetFormattedText());
			return true;
		}
		map<string, unsigned int>::iterator it = variables.find(token);
		if (it == variables.end())
		{
			ErrorReporter::AddError(string("Unknown variable \'").append(token).append("\'."), line.GetLineNumber(), line.GetFormattedText());
		}
		else
		{
			unsigned int index = it->second;
			*dest++ = (index & 0xFF);
			*dest++ = (index >> 8) & 0xFF;
			offset += 2;
		}
	}

	else
	{
		ErrorReporter::AddWarning(string("Unknown command parameter \'").append(param).append("\'. Skipping token \'").append(token).append("\'."), line.GetLineNumber(), line.GetFormattedText());
	}
	return true;
}

bool CommandParser::ParseAsInteger(Line& line, unsigned char*& dest, string& token, string& param, unsigned int& offset, bool report_errors)
{
	unsigned int value;
	if (!TokenParser::IsInteger(token, value))
	{
		if (TokenParser::IsVariable(token))
		{
			map<string, unsigned int>::iterator it = variables.find(token);
			if (it != variables.end())
			{
				*dest++ = 1; //1 means it's an int variable
				*dest++ = it->second & 0xFF;
				*dest++ = (it->second >> 8) & 0xFF;
				offset += 3;
				return true;
			}
			else
			{
				if (report_errors)
					ErrorReporter::AddError(string("Unknown variable \'").append(token).append("\'."), line.GetLineNumber(), line.GetFormattedText());
				return false;
			}
		}
		else if (report_errors)
			ErrorReporter::AddError(string("Cannot convert \'").append(token).append("\' to integer."), line.GetLineNumber(), line.GetFormattedText());
		return false;
	}

	unsigned int max = (unsigned int)pow(256, param.length()) - 1;
	//if (value > max)
	//	ErrorReporter::AddWarning(string("Truncation of argument value ").append(to_string(value)).append(" to ").append(to_string(value & max)).append(" (max ").append(to_string(max)).append(")."), line.GetLineNumber(), line.GetFormattedText());
	//value &= max;
	*dest++ = 0; //0 means it's a raw value
	*dest++ = (value & 0xFF);
	*dest++ = (value >> 8) & 0xFF;
	offset += 3;

	return true;
}

bool CommandParser::ParseAsString(Line& line, unsigned char*& dest, string& token, string& param, unsigned int& offset, bool report_errors)
{
	string value;
	if (!TokenParser::IsPureString(token, value))
	{
		if (TokenParser::IsVariable(token))
		{
			map<string, unsigned int>::iterator it = variables.find(token);
			if (it != variables.end())
			{
				*dest++ = 2; //2 means it's a string variable
				*dest++ = it->second & 0xFF;
				*dest++ = (it->second >> 8) & 0xFF;
				offset += 3;
				return true;
			}
			else
			{
				if (report_errors)
					ErrorReporter::AddError(string("Unknown variable \'").append(token).append("\'."), line.GetLineNumber(), line.GetFormattedText());
				return false;
			}
		}
		else if (report_errors)
			ErrorReporter::AddError(string("Cannot convert \'").append(token).append("\' to string."), line.GetLineNumber(), line.GetFormattedText());
		return false;
	}
	else
	{
		*dest++ = 3; //3 means its a string value
		unsigned char* len_dest = dest;
		unsigned int length = value.length();
		dest += 2;
		for (unsigned int i = 0; i < value.length() && i < 65536; i++)
		{
			//escape sequences
			if (value[i] == '\\' && i + 1 < value.length())
			{
				char c = '\\';
				switch (value[i + 1])
				{
				case '\'':
					c = 0x27;
					break;
				case '\"':
					c = 0x22;
					break;
				case '?':
					c = 0x3F;
					break;
				case '\\':
					c = 0x5c;
					break;
				case 'n':
					c = 0xA;
					break;
				case 'r':
					c = 0xd;
					break;
				case 't':
					c = 0x9;
					break;
				case 'v':
					c = 0xb;
					break;
				case 'f':
					c = 0xc;
					break;
				}
				*dest++ = (unsigned char)c;
				offset++;
				i++;
				length--;
				continue;
			}
			*dest++ = (unsigned char)value[i];
			offset++;
		}
		offset += 3;
		*len_dest++ = length & 0xFF;
		*len_dest = (length >> 8) & 0xFF;
	}
	return true;
}

bool CommandParser::ParamIsNumber(string& param)
{
	if (param.length() == 0)
		return false;
	for (unsigned int i = 0; i < param.length(); i++)
	{
		if (param[i] != '#')
			return false;
	}

	return true;
}