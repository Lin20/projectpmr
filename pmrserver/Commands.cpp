#include "Commands.h"

vector<Command> Commands::commands;

Commands::Commands()
{
}

Commands::~Commands()
{
}

void Commands::LoadCommands()
{
	std::ifstream infile(COMMANDS_LOCATION);
	std::string line;
	while (std::getline(infile, line))
	{
		ProcessLine(line);
	}
}

Command* Commands::GetCommand(string& name, unsigned int& start)
{
	for (unsigned int i = start; i < commands.size(); i++)
	{
		if (commands[i].name == name)
			return &commands[i];
	}
	return 0;
}

void Commands::ProcessLine(string src)
{
	if (src.empty())
		return;
	//try to break the line up into tokens
	unsigned char token_index = 0;
	char* token;
	char* line = (char*)src.c_str();
	token = strtok(line, "\t");

	Command c;

	while (token != NULL)
	{
		switch (token_index)
		{
		case 0: //opcode
			c.opcode = atoi(token);
			break;
		case 1: //name
			c.name = string(token);
			break;
		case 2: //desc
			c.description = string(token);
			break;
		default: //parameters
			c.params.push_back(string(token));
			unsigned int v;
			string t = string(token);
			if (!TokenParser::IsInteger(t, v))
				c.args_count++;
			break;
		}
		token_index++;
		token = strtok(0, "\t");

	}

	commands.push_back(c);
}