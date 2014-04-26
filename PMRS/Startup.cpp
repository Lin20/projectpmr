#include <iostream>
#include <string>
#include <fstream>
#include <streambuf>

#include "Compiler.h"

using namespace std;

int main(int count, char** args)
{
	Compiler c(cout, "C:/red dumps/test.txt");

	cin.ignore(2);

	return 0;
}