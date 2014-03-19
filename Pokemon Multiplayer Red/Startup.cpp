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
	Map map(12);
	map.Load();
	Map connected(map.north.map);
	connected.Load();

	sf::RenderWindow window(sf::VideoMode(map.width * 32, map.height * 32), "SFML works!");
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
			tileset.Draw(&window, x, y + 4, map.tiles[y*map.width + x], 4, 4);
		for (int x = 0; x < connected.width; x++)
		{
			for (int y = 0; y < connected.height; y++)
			{
				tileset.Draw(&window, x - (map.north.x_alignment + 1) / 2, y - (map.north.y_alignment + 1) / 2 + 4, connected.tiles[y*connected.width + x], 4, 4);
			}
		}
		window.display();
	}

	return 0;
}