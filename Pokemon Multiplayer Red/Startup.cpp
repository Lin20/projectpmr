#include <fstream>
#include <iostream>

#include <SFML/Graphics.hpp>
#include "TileMap.h"
#include "Map.h"

using namespace std;

int main()
{
	Map map(51);
	map.Load();

	sf::RenderWindow window(sf::VideoMode(map.width * 32, map.height * 32), "SFML works!");
	sf::Texture tileset = sf::Texture();
	tileset.loadFromFile(string("C:\\red dumps\\tilesets\\").append(to_string(map.tileset)).append(".png"));
	ifstream myfile(string("C:\\red dumps\\tilesets\\formation\\").append(to_string(map.tileset)).append(".dat"), ios::in | ios::binary);
	unsigned char data[256 * 4 * 4];
	myfile.read((char*)data, 256 * 4 * 4);
	myfile.close();

	TileMap tilemap(&tileset, data, 16, map.tileset, true);

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
			tilemap.Draw(&window, x, y, map.tiles[y*map.width + x], 4, 4);
		window.display();
	}

	return 0;
}