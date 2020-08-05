#include "ErrorReporter.h"

vector<unsigned int> ErrorReporter::lines;
vector<string> ErrorReporter::messages;

ErrorReporter::ErrorReporter()
{
}

ErrorReporter::~ErrorReporter()
{
}

void ErrorReporter::AddWarning(string warning, unsigned int line, string& formatted_line)
{
	string header = "[";
	header.append(itos(messages.size() + 1));
	header.append("] ");
	header.append("Warning on line ");
	header.append(itos(line + 1));
	header.append(": ");
	warning.insert(0, header);
	warning.append("\n");
	warning.append(formatted_line);

	messages.push_back(warning);
}

void ErrorReporter::AddWarning(const char* warning, unsigned int line, string& formatted_line)
{
	string e = string(warning);
	AddWarning(e, line, formatted_line);
}

void ErrorReporter::AddError(string error, unsigned int line, string& formatted_line)
{
	string header = "[";
	header.append(itos(messages.size() + 1));
	header.append("] ");
	header.append("Error on line ");
	header.append(itos(line + 1));
	header.append(": ");
	error.insert(0, header);
	error.append("\n");
	error.append(formatted_line);

	messages.push_back(error);
	lines.push_back(line);
}

void ErrorReporter::AddError(const char* error, unsigned int line, string& formatted_line)
{
	string e = string(error);
	AddError(e, line, formatted_line);
}

bool ErrorReporter::Printout(ostream& out)
{
	for (unsigned int i = 0; i < messages.size(); i++)
	{
		out << messages[i] << "\n\n";
	}

	return lines.size() > 0;
}