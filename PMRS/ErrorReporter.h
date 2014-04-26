#pragma once

#include <vector>
#include <string>
#include <sstream>
#include <ostream>

using namespace std;

class ErrorReporter
{
public:
	ErrorReporter();
	~ErrorReporter();

	static void AddWarning(const string warning, unsigned int line, string& formatted_line);
	static void AddWarning(const char* warning, unsigned int line, string& formatted_line);
	static void AddError(const string error, unsigned int line, string& formatted_line);
	static void AddError(const char* error, unsigned int line, string& formatted_line);
	inline static unsigned int GetMessageCount() { return messages.size(); }
	inline static unsigned int GetErrorCount() { return lines.size(); }

	static bool Printout(ostream& out);

private:
	static vector<unsigned int> lines;
	static vector<string> messages;

	static std::string itos(int i)
	{
		std::ostringstream temp;
		temp << i;
		return temp.str();
	}
};

