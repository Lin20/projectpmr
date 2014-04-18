#pragma once

struct DataBlock
{
	inline DataBlock(unsigned int size = 0)
	{
		if (size > 0)
			this->data = new unsigned char[size];
		else
			this->data = 0;
		this->data_start = this->data;
		this->size = size;
	}

	inline DataBlock(unsigned char* data, unsigned int size = 0)
	{
		this->data = data;
		this->data_start = data;
		this->size = size;
	}

	inline ~DataBlock()
	{
		if (data_start)
			delete[] data_start;
	}

	unsigned char* data;
	unsigned char* data_start; //used to delete[] data when data is modified for reading
	unsigned int size;
};