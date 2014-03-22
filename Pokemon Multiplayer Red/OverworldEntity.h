#include <SFML/Graphics.hpp>

#include "TileMap.h"
#include "ResourceCache.h"

class OverworldEntity : public TileMap
{
public:
	OverworldEntity(unsigned char index, unsigned char x, unsigned char y, unsigned int direction);
	~OverworldEntity();

	void Update();
	void Render(sf::RenderWindow* window);

private:
	unsigned char index;
	int x;
	int y;
};