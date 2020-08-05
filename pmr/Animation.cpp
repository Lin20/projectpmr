#include "Animation.h"


Animation::Animation(unsigned char _index, unsigned char _pokemon)
{
	index = _index;
	pokemon = _pokemon;
	is_done = false;
	hide_pokemon = false;
	timer = 0;
}


Animation::~Animation()
{
}

void Animation::DrawPokemon(sf::RenderWindow* window, unsigned char x, unsigned char y)
{

}
void Animation::DrawMove(sf::RenderWindow* window, unsigned char x, unsigned char y)
{

}
void Animation::DrawScreen(sf::RenderWindow* window, unsigned char x, unsigned char y)
{

}
