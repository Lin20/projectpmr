#ifdef _WIN32
#ifdef _DEBUG
#define CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>
#endif
#endif

#include <fstream>
#include <iostream>
#include <ctime>

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include "Common.h"
#include "Constants.h"
#include "TileMap.h"
#include "Map.h"
#include "Tileset.h"
#include "MapConnection.h"

#include "Engine.h"

using namespace std;

int main()
{
	//_crtBreakAlloc = 6132;
	Engine engine;

	sf::RenderWindow window(sf::VideoMode(VIEWPORT_WIDTH * 16, VIEWPORT_HEIGHT * 16), "SFML works!");

	window.setFramerateLimit(60);


	while (window.isOpen())
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();
		}

		window.clear();
		engine.Update();
		engine.Render(&window);
		window.display();
	}

	Players::ReleaseResources();
	MenuCache::ReleaseResources();
	ResourceCache::ReleaseResources();
	engine.~Engine(); //this makes memory leak detection easier

#ifdef _WIN32
#ifdef _DEBUG
	_CrtDumpMemoryLeaks();
#endif
#endif


	return 0;
}
