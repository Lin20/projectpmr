#include <SFML/Graphics.hpp>

#include "TileMap.h"
#include "ResourceCache.h"

class OverworldEntity : TileMap
{
public:
	OverworldEntity(ResourceCache& resources, unsigned char index, unsigned char x, unsigned char y, unsigned int direction);
	~OverworldEntity();

	void Update();
	void Render(sf::RenderWindow* window);

private:
	unsigned char index;
	int x;
	int y;
};