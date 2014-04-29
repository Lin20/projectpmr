#include "Map.h"

Map::Map()
{
}

Map::Map(unsigned char index,vector<OverworldEntity*>* scene_entities)
{
	this->index = index;
	this->scene_entities = scene_entities;
	tiles = 0;
	palette = ResourceCache::GetPalette(0);
	for(int i = 0; i < 4; i++)
		connected_maps[i] = 0;
}

Map::~Map()
{
	if (tiles)
		delete[] tiles;
	for (int i = 0; i < 4; i++)
	{
		if (connected_maps[i])
			delete connected_maps[i];
	}
}

bool Map::Load(bool only_load_tiles)
{
	DataBlock* data = ReadFile(ResourceCache::GetResourceLocation(string("maps/").append(itos(index).append(".dat"))).c_str());
	if (!ParseHeader(data, only_load_tiles))
	{
		delete data;
		return false;
	}
	delete data;
	LoadPalette();
	return true;
}

void Map::LoadPalette()
{
	unsigned char pal_index = ResourceCache::GetMapPaletteIndex(index);
	if (pal_index != 0xFF)
		palette = ResourceCache::GetPalette(pal_index);
}

bool Map::ParseHeader(DataBlock* data, bool only_load_tiles)
{
	if (!data || data->size < 3)
		return false;

	unsigned char* p = data->data;
	tileset = *p++;
	height = *p++;
	width = *p++;

	if (tiles)
		delete[] tiles;
	tiles = new unsigned char[width * height];
	if (data->size - 3 < (unsigned int)(width * height))
		return true;
	memcpy(tiles, p, width * height);
	if (only_load_tiles)
		return true;
	p += width * height;

	connection_mask = *p++;
	for (int b = 3; b >= 0; b--)
	{
		if ((connection_mask & (1 << b)) != 0)
		{
			connections[3 - b].map = *p++;
			connections[3 - b].y_alignment = *p++;
			connections[3 - b].x_alignment = *p++;
		}
	}
	for (int i = 0; i < 4; i++)
	{
		if (HasConnection(i))
		{
			if (!connected_maps[i])
				connected_maps[i] = new Map(connections[i].map, scene_entities);
			else
				connected_maps[i]->index = connections[i].map;
			connected_maps[i]->Load(true);
			if (connected_maps[i]->width * connected_maps[i]->height >= 128 * 128) //bad map
			{
				connected_maps[i] = 0;
				connection_mask ^= 1 << (3 - i);
			}
		}
		else
			connected_maps[i] = 0;
	}

	border_tile = *p++;
	unsigned char count = *p++;
	warps.clear();
	for (int i = 0; i < count; i++)
	{
		Warp w;
		w.x = *p++;
		w.y = *p++;
		w.dest_point = *p++;
		w.dest_map = *p++;
		warps.push_back(w);
	}

	count = *p++;
	signs.clear();
	for (int i = 0; i < count; i++)
	{
		Sign s;
		s.x = *p++;
		s.y = *p++;
		s.text = *p++;
		signs.push_back(s);
	}

	count = *p++;
	entities.clear();
	for (int i = 0; i < count; i++)
	{
		Entity e;
		e.sprite = *p++;
		e.x = *p++ - 4;
		e.y = *p++ - 4;
		e.movement1 = *p++;
		e.movement2 = *p++;
		e.text = *p++;
		if ((e.text & 0x40) != 0)
		{
			e.trainer = *p++;
			e.pokemon_set = *p++;
		}
		else if ((e.text & 0x80) != 0)
		{
			e.item = *p++;
		}
		entities.push_back(e);
	}

	return true;
}

unsigned char Map::Get8x8Tile(int x, int y)
{
	return GetCornerTile(x / 2, y / 2, (abs(x) % 2) + (abs(y) % 2) * 2);
	/*Tileset* tileset = ResourceCache::GetTileset(this->tileset);
	if (!tileset)
		return 0;
	if (x < 0 || y < 0 || x >= width * 4 || y >= height * 4)
	{
		return tileset->GetTile8x8(border_tile, x % 4 + (y % 4) * 4);
	}
	return tileset->GetTile8x8(tiles[x / 4 + y / 4 * width], x % 4 + (y % 4) * 4);*/
}

unsigned char Map::GetCornerTile(int x, int y, unsigned char corner)
{
	Tileset* tileset = ResourceCache::GetTileset(this->tileset);
	if (!tileset)
		return 0;
	if (x < 0 || y < 0 || x >= width * 2 || y >= height * 2)
	{
		if (x < 0 && HasConnection(CONNECTION_WEST))
		{
			return connected_maps[CONNECTION_WEST]->GetCornerTile(connected_maps[CONNECTION_WEST]->width * 2 - 1, y + connections[CONNECTION_WEST].y_alignment, corner);
		}
		if (x >= width * 2 && HasConnection(CONNECTION_EAST))
		{
			return connected_maps[CONNECTION_EAST]->GetCornerTile(0, y + connections[CONNECTION_EAST].y_alignment, corner);
		}
		if (y < 0 && HasConnection(CONNECTION_NORTH))
		{
			return connected_maps[CONNECTION_NORTH]->GetCornerTile(x + connections[CONNECTION_NORTH].x_alignment, connected_maps[CONNECTION_NORTH]->height * 2 - 1, corner);
		}
		if (y >= height * 2 && HasConnection(CONNECTION_SOUTH))
		{
			return connected_maps[CONNECTION_SOUTH]->GetCornerTile(x + connections[CONNECTION_SOUTH].x_alignment, 0, corner);
		}
		corner = (x % 2 == 0 ? 0 : 2) + corner % 2 + (y % 2 == 0 ? 0 : 8) + (corner / 2) * 4;
		return tileset->GetTile8x8(border_tile, corner);
	}
	corner = (x % 2 == 0 ? 0 : 2) + corner % 2 + (y % 2 == 0 ? 0 : 8) + (corner / 2) * 4;
	return tileset->GetTile8x8(tiles[x / 2 + y / 2 * width], corner);
}

bool Map::IsPassable(int x, int y, OverworldEntity* ignore, bool entity_clipping)
{
	Tileset* tileset = ResourceCache::GetTileset(this->tileset);
	if (!tileset)
		return true;
	unsigned char tile = GetCornerTile(x, y, 2);

	DataBlock* collision = tileset->GetCollisionData();
	if (!collision)
		return true;

	if (scene_entities && entity_clipping)
	{
		for (unsigned int i = 0; i < scene_entities->size(); i++)
		{
			if (!(*scene_entities)[i])
				continue;
			OverworldEntity* e = (*scene_entities)[i];
			if (e == ignore)
				continue;
			int ex = e->x / 16;
			int ey = e->y / 16;
			if (x == ex && y == ey)
				return false;
			ex += DELTAX(e->GetMovementDirection());
			ey += DELTAY(e->GetMovementDirection());
			if (x == ex && y == ey)
				return false;
			ex = (e->x + 15) / 16;
			ey = (e->y + 15) / 16;
			if (x == ex && y == ey)
				return false;
		}
	}

	//the original game uses the lower-left tile of a 16x16 block to determine whether or not that block is passable
	for (unsigned int i = 0; i < collision->size; i++)
	{
		if (collision->data[i] == tile)
			return true;
	}

	return false;
}

bool Map::CanJump(int x, int y, unsigned char direction)
{
	if (x < 0 || y < 0 || x >= width * 2 || y >= height * 2)
		return false;
	if (this->tileset > 0)
		return false;
	Tileset* tileset = ResourceCache::GetTileset(this->tileset);
	if (!tileset)
		return true;
	DataBlock* collision = tileset->GetCollisionData();
	if (!collision)
		return true;

	//the original game uses the lower-left tile of a 16x16 block to determine whether or not that block is jumpable
	unsigned char standing_on = tileset->GetTile8x8(tiles[x / 2 + y / 2 * width], (x % 2 == 0 ? 0 : 2) + (y % 2 == 0 ? 4 : 12));
	x += DELTAX(direction);
	y += DELTAY(direction);
	unsigned char next = tileset->GetTile8x8(tiles[x / 2 + y / 2 * width], (x % 2 == 0 ? 0 : 2) + (y % 2 == 0 ? 4 : 12));

	unsigned char* p = ResourceCache::GetLedges()->data;
	while (p < ResourceCache::GetLedges()->data + ResourceCache::GetLedges()->size)
	{
		if (*p == 0xFF)
			return false;
		if (*p++ / 4 != direction)
		{
			p += 3;
			continue;
		}
		if (*p++ != standing_on)
		{
			p += 2;
			continue;
		}
		if (*p++ != next)
		{
			p++;
			continue;
		}
		p++; //button input
		return true;
	}

	return false;
}

bool Map::InGrass(int x, int y)
{
	if (x < 0 || y < 0 || x >= width * 2 || y >= height * 2)
		return false;
	Tileset* tileset = ResourceCache::GetTileset(this->tileset);
	if (!tileset)
		return true;
	DataBlock* collision = tileset->GetCollisionData();
	if (!collision)
		return true;

	//the original game uses the lower-left tile of a 16x16 block to determine whether or not that block is passable
	return GetCornerTile(x, y, 2) == tileset->GetMiscData()->data[3]; //second to last byte in the tileset header is the grass tile
}

bool Map::CanWarp(int x, int y, unsigned char direction, Warp* check_warp)
{
	if (!check_warp)
		return false;
	x += DELTAX(direction);
	y += DELTAY(direction);
	if (x < 0 || y < 0 || x >= width * 2 || y >= height * 2)
	{
		check_warp->type = WARP_TO_OUTSIDE;
		return true;
	}
	x -= DELTAX(direction);
	y -= DELTAY(direction);
	check_warp->type = WARP_TILE;

	Tileset* tileset = ResourceCache::GetTileset(this->tileset);
	if (!tileset)
		return false;

	bool b = tileset->IsDoorTile(tileset->GetTile8x8(tiles[x / 2 + y / 2 * width], (x % 2 == 0 ? 0 : 2) + (y % 2 == 0 ? 4 : 12)));
	if (b)
		return true;
	x += DELTAX(direction);
	y += DELTAY(direction);
	if (!IsPassable(x, y))
	{
		return tileset->IsDoorTile(tileset->GetTile8x8(tiles[x / 2 + y / 2 * width], (x % 2 == 0 ? 0 : 2) + (y % 2 == 0 ? 4 : 12))) || (index > OUTSIDE_MAP && tileset->GetTile8x8(tiles[x / 2 + y / 2 * width], (x % 2 == 0 ? 0 : 2) + (y % 2 == 0 ? 4 : 12)) == tileset->GetTile8x8(border_tile, (x % 2 == 0 ? 0 : 2) + (y % 2 == 0 ? 4 : 12)) && direction == ENTITY_DOWN);
	}
	return false;
}

void Map::RenderRectangle(int x, int y, int width, int height, sf::Sprite& sprite, sf::RenderWindow* window)
{
	int delta_y = (y < 0 ? (y - 7) / 8 * 8 + y % 8 : 0);
	y -= delta_y;
	sf::IntRect src_rect;
	int _px = x;
	int _py = y;
	for (; x <= _px + width; x += 8)
	{
		int lX = x / 8 * 8;
		for (y = _py; y <= _py + height; y += (y % 8 != 0 ? y % 8 : 8))
		{
			unsigned char tile = Get8x8Tile(x / 8, y / 8);
			int w = 8 - (lX < _px ? x % 8 : lX + 8 > _px + width ? 8 - x % 8 : 0);
			int h = 8 - y % 8;
			if (y + h > _py + height)
				h = _py + height - y;
			src_rect.left = (tile % 16) * 8 + (8 - w);
			src_rect.top = (tile / 16) * 8 + y % 8;
			src_rect.width = w;
			src_rect.height = h;
			sprite.setTextureRect(src_rect);
			sprite.setPosition((float)(lX + 8 - w), (float)y);
			window->draw(sprite);
		}
	}
}

/*
int lY = (y < 0 ? y - (8 + (y % 8)) : y) / 8 * 8;
unsigned char tile = Get8x8Tile(x / 8, lY / 8);
int w = 8 - (lX < _px ? x % 8 : lX + 8 > _px + width ? 8 - x % 8 : 0);
int h = (lY < _py ? y % 8 : lY + 8 > _py + height ? 8 - y % 8 : 0);
if (lY + h > _py + height)
h = _py + height - lY;
src_rect.left = (tile % 16) * 8 + (8 - w);
src_rect.top = (tile / 16) * 8 + (8 - h) - (delta_y > 0 ? 8 + (delta_y % 8) : 0);
src_rect.width = w;
src_rect.height = h;
sprite.setTextureRect(src_rect);
sprite.setPosition((float)(lX + 8 - w), (float)(lY + 8 - h));
window->draw(sprite);
*/