#pragma once

#include <SFML/Graphics.hpp>

class Animation
{
public:
	Animation(unsigned char index, unsigned char pokemon);
	~Animation();

	virtual void DrawPokemon(sf::RenderWindow* window, unsigned char x, unsigned char y);
	virtual void DrawMove(sf::RenderWindow* window, unsigned char x, unsigned char y);
	virtual void DrawScreen(sf::RenderWindow* window, unsigned char x, unsigned char y);

private:
	unsigned char index;
	unsigned char pokemon;

	bool is_done;
	bool hide_pokemon;
	unsigned char timer;
};
