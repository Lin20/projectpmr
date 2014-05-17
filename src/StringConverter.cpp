#include "StringConverter.h"

/*
TABLE OF ASCII POKESTRING EQUIVALENTS
Note: These can all be changed in ascii_table.dat
------------------------------------------------
#	=	POKé
*	=	x
$	=	pokedollar
\r	=	paragraph (prompt for input)
\n	=	new line
\f	=	prompt for input to continue event
\v	=	scroll to next line
\a	=	done (action)
\t	=	delay
-------------------------------------------------
*/

string& pokestring(std::string& src)
{
	//we will convert the regular ascii table into a "pokestring"
	//in other words, we'll convert src to a be an array of tiles.
	//i goes to src.length - 1. the ascii table has 00 convert to $57
	//for text ending, but it's unnecessary for code-based strings
	for (unsigned int i = 0; i < src.length(); i++)
	{
		unsigned char c = src[i];
		src[i] = ResourceCache::GetAsciiTable()->data[c];

		//TODO: Replace with constants; make not really ugly
		switch (c)
		{
		case (unsigned char)'\'': //'x
			char next = (i + 1 < src.length() ? src[i + 1] : 0);
			if (next == 's')
			{
				src[i] = (char)0xBD;
				src.erase(src.begin() + i + 1, src.begin() + i + 2);
			}
			if (next == 't')
			{
				src[i] = (char)0xBE;
				src.erase(src.begin() + i + 1, src.begin() + i + 2);
			}
			else if (next == '\'')
			{
				src[i] = (char)0xE0;
				src.erase(src.begin() + i + 1, src.begin() + i + 2);
			}
			break;
		}

		if ((unsigned char)src[i] >= 0x80) //anything over 0x7F is a single char
			continue;

		c = src[i];
		switch (c)
		{
		case MESSAGE_POKE: //POKé
			src.erase(i, 1);
			src.insert(i--, "POKé");
			break;
		}
	}
	
	return src;
}

string pokestring(const char* c)
{
	string s(c);
	pokestring(s);
	return s;
}