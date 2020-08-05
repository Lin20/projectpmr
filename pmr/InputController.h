#pragma once

#include <SFML/Window.hpp>

class InputController
{
public:
	InputController();
	~InputController();

	static bool KeyDownOnce(sf::Keyboard::Key k)
	{
		bool b = last_keys[(int)k];
		last_keys[(int)k] = sf::Keyboard::isKeyPressed(k);
		return !b && last_keys[(int)k];
	}

	static bool KeyDown(sf::Keyboard::Key k)
	{
		return last_keys[(int)k] = sf::Keyboard::isKeyPressed(k);
	}

private:
	static bool last_keys[256];
};

