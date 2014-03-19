#pragma once

struct DataBlock
{
	inline DataBlock(unsigned char* data = 0, unsigned int size = 0)
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