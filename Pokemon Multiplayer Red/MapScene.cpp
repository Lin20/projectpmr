#include "MapScene.h"


MapScene::MapScene() : Scene()
{
	active_map = 0;

	Focus(STARTING_X, STARTING_Y);
	SwitchMap(STARTING_MAP);
}

MapScene::~MapScene()
{
	if (active_map)
		delete active_map;
	delete test_entity;
}

/// <summary>
/// Updates this instance.
/// </summary>
void MapScene::Update()
{
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Down))
		test_entity->Move(ENTITY_DOWN, 1);
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Up))
		test_entity->Move(ENTITY_UP, 1);
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Left))
		test_entity->Move(ENTITY_LEFT, 1);
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Right))
		test_entity->Move(ENTITY_RIGHT, 1);
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::F1))
	{
		test_entity->x = STARTING_X * 16;
		test_entity->y = STARTING_Y * 16;
		SwitchMap(STARTING_MAP);
	}

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Space) && !key_down)
	{
		key_down = true;
		SwitchMap((active_map->index + 1) % 256);
		return;
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::LShift) && !key_down)
	{
		key_down = true;
		SwitchMap((active_map->index + 255) % 256);
		return;
	}
	if (!sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Space) && !sf::Keyboard::isKeyPressed(sf::Keyboard::Key::LShift))
		key_down = false;

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
		viewport.move(-8, 0);
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
		viewport.move(8, 0);
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
		viewport.move(0, -8);
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
		viewport.move(0, 8);

	int x = (int)viewport.getCenter().x;
	int y = (int)viewport.getCenter().y;

	if (x < 0 && active_map->HasConnection(CONNECTION_WEST))
	{
		MapConnection connection = active_map->connections[CONNECTION_WEST];
		SwitchMap(active_map->connections[CONNECTION_WEST].map);
		FocusFree(active_map->width * 32 + 16, y + (connection.y_alignment + (connection.y_alignment < 0 ? -1 : -1)) * 16);
	}
	else if (x >= active_map->width * 32 && active_map->HasConnection(CONNECTION_EAST))
	{
		MapConnection connection = active_map->connections[CONNECTION_EAST];
		SwitchMap(active_map->connections[CONNECTION_EAST].map);
		FocusFree(16, y + (connection.y_alignment + (connection.y_alignment < 0 ? -1 : -1)) * 16);
	}

	if (y < 0 && active_map->HasConnection(CONNECTION_NORTH))
	{
		MapConnection connection = active_map->connections[CONNECTION_NORTH];
		SwitchMap(active_map->connections[CONNECTION_NORTH].map);
		FocusFree(x + (connection.x_alignment + (connection.x_alignment < 0 ? 1 : 1)) * 16, active_map->height * 32 - 20);
	}
	else if (y >= active_map->height * 32 && active_map->HasConnection(CONNECTION_SOUTH))
	{
		MapConnection connection = active_map->connections[CONNECTION_SOUTH];
		SwitchMap(active_map->connections[CONNECTION_SOUTH].map);
		FocusFree(x + (connection.x_alignment + (connection.x_alignment < 0 ? 1 : 1)) * 16, -16);
	}

	Tileset* tex = ResourceCache::GetTileset(active_map->tileset);
	if (tex)
		tex->AnimateTiles();
	test_entity->Update();
}

void MapScene::Render(sf::RenderWindow* window)
{
	FocusFree(test_entity->x, test_entity->y);
	window->setView(viewport);

	if (active_map)
	{
		DrawMap(window, *active_map, -1, 0);
		for (int i = 0; i < 4; i++)
		{
			if (active_map->HasConnection(i))
			{
				Map* c = active_map->connected_maps[i];
				c->Load();
				DrawMap(window, *c, i, &active_map->connections[i]);
			}
		}
	}

	test_entity->Render(window);
}

void MapScene::SwitchMap(unsigned char index)
{
	unsigned char previous_palette = 0;
	if (!active_map)
	{
		active_map = new Map(index);
	}
	else
	{
		previous_palette = ResourceCache::GetMapPaletteIndex(active_map->index);
		active_map->index = index;
	}
	if (!active_map->Load())
	{
#ifdef _DEBUG
		cout << "FATAL: Failed to load map " << index << "!";
		return;
#endif
	}
	if (!test_entity)
		test_entity = new OverworldEntity(active_map, 1, STARTING_X, STARTING_Y, ENTITY_DOWN);

	Tileset* tex = ResourceCache::GetTileset(active_map->tileset);

	if (tex)
	{
		tex->SetPalette(active_map->GetPalette());
		for (int i = 0; i < 4; i++)
		{
			if (active_map->connected_maps[i])
				ResourceCache::GetTileset(active_map->connected_maps[i]->tileset)->SetPalette(active_map->GetPalette());
		}
	}
}

void MapScene::Focus(signed char x, signed char y)
{
	viewport.reset(sf::FloatRect((float)(int)(x - VIEWPORT_WIDTH / 2 + 1) * 16, (float)(int)(y - VIEWPORT_HEIGHT / 2) * 16, VIEWPORT_WIDTH * 16, VIEWPORT_HEIGHT * 16));
}

void MapScene::FocusFree(int x, int y)
{
	viewport.reset(sf::FloatRect(x - (int)(VIEWPORT_WIDTH / 2 - 1) * 16, y - ((int)(VIEWPORT_HEIGHT / 2)) * 16, VIEWPORT_WIDTH * 16, VIEWPORT_HEIGHT * 16));
}

void MapScene::DrawMap(sf::RenderWindow* window, Map& map, int connection_index, MapConnection* connection)
{
	int startX = (int)(viewport.getCenter().x - viewport.getSize().x / 2) / 32;
	int startY = (int)(viewport.getCenter().y - viewport.getSize().y / 2) / 32;
	int endX = (int)(viewport.getCenter().x + viewport.getSize().x / 2) / 32;
	int endY = (int)(viewport.getCenter().y + viewport.getSize().y / 2) / 32;

	switch (connection_index)
	{
	case CONNECTION_NORTH:
		startX += (connection->x_alignment + 1) / 2;
		endX += (connection->x_alignment + 1) / 2;
		startY += ((unsigned char)connection->y_alignment + 1) / 2;
		endY += ((unsigned char)connection->y_alignment + 1) / 2;

		endY = min(endY, map.height - 1);
		break;
	case CONNECTION_SOUTH:
		startX += (connection->x_alignment + 1) / 2;
		endX += (connection->x_alignment + 1) / 2;
		startY += ((unsigned char)connection->y_alignment + 1) / 2;
		endY += ((unsigned char)connection->y_alignment + 1) / 2;

		startY -= active_map->height;
		endY -= active_map->height;
		break;
	case CONNECTION_WEST:
		startX += ((unsigned char)connection->x_alignment + 1) / 2;
		endX += ((unsigned char)connection->x_alignment + 1) / 2;
		startY += (connection->y_alignment + 1) / 2;
		endY += (connection->y_alignment + 1) / 2;

		endX = min(endX, map.width - 1);
		break;
	case CONNECTION_EAST:
		startX += ((unsigned char)connection->x_alignment + 1) / 2;
		endX += ((unsigned char)connection->x_alignment + 1) / 2;
		startY += (connection->y_alignment + 1) / 2;
		endY += (connection->y_alignment + 1) / 2;

		startX -= active_map->width;
		endX -= active_map->width;
		break;
	}

	Tileset* tileset = ResourceCache::GetTileset(map.tileset); //isn't it great having the textures be small and cached so we don't have to worry about loading once and passing them around? :D
	if (!tileset)
		return;
	for (int x = startX - 1; x <= endX; x++)
	{
		for (int y = startY - 1; y <= endY; y++)
		{
			unsigned char tile = map.border_tile;
			if (x >= 0 && x < map.width && y >= 0 && y < map.height) //are we drawing a piece of the map or a border tile
				tile = map.tiles[y * map.width + x];
			else if (connection_index != -1) //don't draw border tiles for connected maps
				continue;
			int drawX = x;
			int drawY = y;
			if (connection)
			{
				switch (connection_index)
				{
				case CONNECTION_NORTH: //north
				case CONNECTION_SOUTH: //south
					drawX -= (connection->x_alignment + 1) / 2;
					drawY -= ((unsigned char)connection->y_alignment + 1) / 2;
					if (connection->x_alignment < 0)
						drawX++;
					break;

				case CONNECTION_WEST: //west
				case CONNECTION_EAST:
					drawX -= ((unsigned char)connection->x_alignment + 1) / 2;
					drawY -= (connection->y_alignment + 1) / 2;
					if (connection->y_alignment < 0)
						drawY++;
					break;
				}

				switch (connection_index)
				{
				case CONNECTION_SOUTH: //south
					drawY += active_map->height;
					break;
				case CONNECTION_EAST: //west
					drawX += active_map->width;
					break;
				}
			}

			tileset->Render(window, drawX, drawY, tile, 4, 4);
		}
	}
}