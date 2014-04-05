#include "StringConverter.h"

void pokestring(std::string& src)
{
	//we will convert the regular ascii table into a "pokestring"
	//in other words, we'll convert src to a be an array of tiles
	for (unsigned int i = 0; i < src.length(); i++)
	{
		unsigned char c = src[i];
		src[i] = ResourceCache::GetAsciiTable()->data[c];
		if (src[i] >= 0x80) //anything over 0x7F is a single char
			continue;
	}
}