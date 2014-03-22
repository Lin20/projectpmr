#pragma once

struct DataBlock
{
	inline DataBlock(unsigned int size = 0)
	{
		if (size > 0)
			this->data = new unsigned char[size];
		else
			this->data = 0;
		this->size = size;
	}

	inline DataBlock(unsigned char* data, unsigned int size = 0)
	{
		this->data = data;
		this->size = size;
	}

	inline ~DataBlock()
	{
		if (data)
			delete[] data;
	}

	unsigned char* data;
	unsigned int size;
};