#include <fstream>
#include <iostream>

#include <SFML/Graphics.hpp>
#include "TileMap.h"
#include "Map.h"
#include "Tileset.h"
#include "MapConnection.h"

using namespace std;

int main()
{
	Map map(10);
	map.Load();
	Map north(map.connections[0].map);
	north.Load();
	Map south(map.connections[1].map);
	south.Load();
	Map east(map.connections[2].map);
	east.Load();
	Map west(map.connections[3].map);
	west.Load();
	ResourceCache cache;
	cache.LoadAll();
	sf::RenderWindow window(sf::VideoMode(1024, 1024), "SFML works!");
	Tileset tileset(map.tileset);

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
		for (int x = 0; x < map.width; x++)
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
		}
		window.display();
	}

	return 0;
}