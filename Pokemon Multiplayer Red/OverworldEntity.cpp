#include "OverworldEntity.h"

OverworldEntity::OverworldEntity(ResourceCache& resources, unsigned char index, unsigned char x, unsigned char y, unsigned int direction) : TileMap()
{
	this->index = index;
	this->x = x * 16;
	this->y = y * 16;
	
	this->tiles_tex = resources.GetEntityTexture(index);
	this->formation = new DataBlock(4);

	//is this entity a moving npc or a static image (eg. pokeball)
	direction = (direction > 3 ? 0 : index <= ENTITY_LIMIT ? direction : 0);
	for (int i = 0; i < 4; i++)
		formation->data[i] = direction * 4 + i;
}

OverworldEntity::~OverworldEntity()
{
	if (formation)
		delete formation;
}