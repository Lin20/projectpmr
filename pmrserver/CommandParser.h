#pragma once

#include <map>
#include "Line.h"
#include "TokenParser.h"
#include "Commands.h"
#include "Label.h"

class CommandParser
{
public:
	CommandParser();
	~CommandParser();

	static void ProcessCommandStage1(unsigned char*& dest, Line& line, unsigned int& offset);
	static void ProcessCommandStage2(unsigned char*& dest); //used for filling in labels
	static unsigned int GetSpaceRequired(Line& l);
	static bool ProcessToken(Line& line, unsigned char*& dest, string& token, string& param, unsigned int& offset);
	static void ProcessTokenStage2(Line& line, unsigned char*& dest, string& token, string& param);
	static bool ParseAsInteger(Line& line, unsigned char*& dest, string& token, string& param, unsigned int& offset, bool report_errors);
	static bool ParseAsString(Line& line, unsigned char*& dest, string& token, string& param, unsigned int& offset, bool report_errors);
	//static bool ParseAsInt(Line& line, unsigned char* dest, string& token, string& param);

private:
	static map<string, unsigned int> labels;
	static map<string, unsigned int> variables;
	static vector<Label> fillin_labels; //the labels referenced in commands. mostly exists for a performance aid
	static bool ParamIsNumber(string& param);
};