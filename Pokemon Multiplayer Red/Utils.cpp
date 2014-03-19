#pragma once

#include "Utils.h"

DataBlock* ReadFile(const char* filename)
{
	std::ifstream myfile = std::ifstream(filename, std::ios::in | std::ios::binary | std::ios::ate);
	if (!myfile)
		return 0;
	unsigned int size = (unsigned int)myfile.tellg();
	myfile.seekg(0, std::ios::beg);
	unsigned char* data = new unsigned char[size];
	myfile.read((char*)data, size);
	myfile.close();
	return new DataBlock(data, size);
}