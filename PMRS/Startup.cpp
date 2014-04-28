#include <iostream>
#include <string>
#include <fstream>
#include <streambuf>

#include "Compiler.h"

using namespace std;

string compile_dir = "";
bool close_when_done = true;
bool silent = false;
bool debug = false;
string bin_dir = "";
string bin_name = "";

void PrintOptions();
void ParseArgs(int count, char** args);
Compiler* compiler;

int main(int count, char** args)
{
	cout << "Pokemon Multiplayer Red Script Compiler v1.00\n\n";
	ParseArgs(count, args);
	if (compile_dir.empty())
		return 1;
	//compile_dir = "C:\\red dumps\\test.txt";
	//bin_dir = "C:\\red dumps\\";
	//bin_name = "test.dat";
	Compiler c(cout, compile_dir.c_str(), bin_dir.append(bin_name), silent, debug);
	c.Compile();

	if (!close_when_done)
		cin.ignore(2);

	return 0;
}

void PrintOptions()
{
	cout << "Argument options:\n";
	cout << "-w	Keeps the console window open after compiling.\n";
	cout << "-s	Compiles the script without printing errors and warnings.\n";
	cout << "-d	Prints out script debug information.\n";
	cout << "-b	Sets the script's compiled directory (must exist).\n\n";
	cout << "-c	Sets the script's compiled name.\n\n";
}

void ParseArgs(int count, char** args)
{
	if (count < 2)
	{
		cout << "Usage: <filename> [options]\n";
		PrintOptions();
		return;
	}
	compile_dir = args[1];
	bin_dir = TokenParser::GetDirectory(compile_dir).append("/");
	bin_name = TokenParser::GetFilename(compile_dir).append(".dat");
	for (int i = 2; i < count; i++)
	{
		string s = TokenParser::ToLower(args[i]);
		if (s == "-w")
		{
			close_when_done = false;
		}
		else if (s == "-s")
		{
			silent = true;
		}
		else if (s == "-d")
		{
			debug = true;
		}
		else if (s == "-b")
		{
			if (i + 1 == count)
			{
				cout << "-b Usage: -b <directory>\n";
			}
			else
			{
				bin_dir = args[++i];
			}
		}
		else if (s == "-c")
		{
			if (i + 1 == count)
			{
				cout << "-c Usage: -c <filename>\n";
			}
			else
			{
				bin_name = args[++i];
			}
		}
		else
		{
			cout << "Unknown option " << s << ".\n";
			PrintOptions();
		}
	}
}