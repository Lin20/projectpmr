#include <iostream>
#include <string>

using namespace std;

int main(int count, char** args)
{
	cout << (count - 1) << " args: ";
	for (int i = 1; i < count; i++)
	{
		cout << args[i] << "; ";
	}
	cout << "\n";
	cin.ignore(2);
	return 0;
}