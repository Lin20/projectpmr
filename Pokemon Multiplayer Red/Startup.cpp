#ifdef _WIN32
#ifdef _DEBUG
#define CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>
#endif
#endif

#include <fstream>
#include <iostream>

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
		/*for (int x = 0; x < map.width; x++)
		for (int y = 0; y < map.height; y++)
			tileset.Draw(&window, x + 4, y + 4, map.tiles[y*map.width + x], 4, 4);
		if (map.HasConnection(0))
		for (int x = 0; x < north.width; x++)
		{
			for (int y = 0; y < north.height; y++)
			{
				tileset.Draw(&window, x + 4 - (map.connections[0].x_alignment + 1) / 2, y - (map.connections[0].y_alignment + 1) / 2 + 4, north.tiles[y*north.width + x], 4, 4);
			}
		}
		if (map.HasConnection(1))
		for (int x = 0; x < south.width; x++)
		{
			for (int y = 0; y < south.height; y++)
			{
				tileset.Draw(&window, x + 4 - (map.connections[1].x_alignment + 1) / 2, y + map.height + 4, south.tiles[y*south.width + x], 4, 4);
			}
		}
		if (map.HasConnection(2))
		for (int x = 0; x < east.width; x++)
		{
			for (int y = 0; y < east.height; y++)
			{
				tileset.Draw(&window, x + 4 - (map.connections[2].x_alignment + 1) / 2, y - (map.connections[2].y_alignment + 1) / 2 + 4, east.tiles[y*east.width + x], 4, 4);
			}
		}
		if (map.HasConnection(3))
		for (int x = 0; x < west.width; x++)
		{
			for (int y = 0; y < west.height; y++)
			{
				tileset.Draw(&window, x + map.width + 4, y - (map.connections[3].y_alignment + 1) / 2 + 4, west.tiles[y*west.width + x], 4, 4);
			}
		}*/
		window.display();
	}

	MenuCache::ReleaseResources();
	ResourceCache::ReleaseResources();
	engine.~Engine(); //this makes memory leak detection easier

#ifdef _WIN32
#ifdef _DEBUG
#define CRTDBG_MAP_ALLOC
	_CrtDumpMemoryLeaks();
#endif
#endif


	return 0;
}