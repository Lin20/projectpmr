#include "Utils.h"
#include "InputController.h"

bool InputController::last_keys[256];

DataBlock* ReadFile(const std::string& filename)
{
	std::ifstream myfile(filename, std::ios::in | std::ios::binary | std::ios::ate);
	if (!myfile)
		return 0;
	unsigned int size = (unsigned int)myfile.tellg();
	myfile.seekg(0, std::ios::beg);
	unsigned char* data = new unsigned char[size];
	myfile.read((char*)data, size);
	myfile.close();
	return new DataBlock(data, size);
}

DataBlock* ReadFile(const char* filename)
{
	std::ifstream myfile(filename, std::ios::in | std::ios::binary | std::ios::ate);
	if (!myfile)
		return 0;
	unsigned int size = (unsigned int)myfile.tellg();
	myfile.seekg(0, std::ios::beg);
	unsigned char* data = new unsigned char[size];
	myfile.read((char*)data, size);
	myfile.close();
	return new DataBlock(data, size);
}

std::string itos(int i)
{
	return std::to_string(i);
	//std::ostringstream temp;
    //temp << i;
    //return temp.str();
}