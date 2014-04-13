#include "MapScene.h"


MapScene::MapScene() : Scene()
{
	active_map = 0;

	//Initialize the player
	entities.push_back(new OverworldEntity(active_map, 1, STARTING_X, STARTING_Y, ENTITY_DOWN, false));
	focus_entity = entities[0];

	Focus(STARTING_X, STARTING_Y);
	SwitchMap(STARTING_MAP);
}

MapScene::~MapScene()
{
	if (active_map)
		delete active_map;
	ClearEntities(true);
}

/// <summary>
/// Updates this instance.
/// </summary>
void MapScene::Update()
{
	current_fade.Update();
	if (!UpdateTextboxes() && current_fade.Done())
	{
		if (sf::Keyboard::isKeyPressed(INPUT_DOWN))
		{
			focus_entity->StartMoving(ENTITY_DOWN);
		}
		else if (sf::Keyboard::isKeyPressed(INPUT_UP))
		{
			focus_entity->StartMoving(ENTITY_UP);
		}
		else if (sf::Keyboard::isKeyPressed(INPUT_LEFT))
		{
			focus_entity->StartMoving(ENTITY_LEFT);
		}
		else if (sf::Keyboard::isKeyPressed(INPUT_RIGHT))
		{
			focus_entity->StartMoving(ENTITY_RIGHT);
		}
		else
			focus_entity->StopMoving();

		if (focus_entity->Snapped() && InputController::KeyDownOnce(INPUT_START))
		{
			ShowTextbox(MenuCache::StartMenu());
			MenuCache::StartMenu()->SetArrowState(ArrowStates::ACTIVE);
		}
	}
	else
		focus_entity->StopMoving();

	int x = (int)(focus_entity ? focus_entity->x : 0);
	int y = (int)(focus_entity ? focus_entity->y : 0);

	if (x < -15 && active_map->HasConnection(CONNECTION_WEST))
	{
		MapConnection connection = active_map->connections[CONNECTION_WEST];
		SwitchMap(active_map->connections[CONNECTION_WEST].map);
		focus_entity->x = active_map->width * 32 - 16;
		focus_entity->y = y + (connection.y_alignment + (connection.y_alignment < 0 ? 0 : 0)) * 16;
		//FocusFree(active_map->width * 32 + 16, y + (connection.y_alignment + (connection.y_alignment < 0 ? -1 : -1)) * 16);
	}
	else if (x >= active_map->width * 32 && active_map->HasConnection(CONNECTION_EAST))
	{
		MapConnection connection = active_map->connections[CONNECTION_EAST];
		SwitchMap(active_map->connections[CONNECTION_EAST].map);
		focus_entity->x = 0;
		focus_entity->y = y + (connection.y_alignment + (connection.y_alignment < 0 ? 0 : 0)) * 16;
		//FocusFree(16, y + (connection.y_alignment + (connection.y_alignment < 0 ? -1 : -1)) * 16);
	}

	if (y < -15 && active_map->HasConnection(CONNECTION_NORTH))
	{
		MapConnection connection = active_map->connections[CONNECTION_NORTH];
		SwitchMap(active_map->connections[CONNECTION_NORTH].map);
		focus_entity->x = x + (connection.x_alignment + (connection.x_alignment < 0 ? 0 : 0)) * 16;
		focus_entity->y = active_map->height * 32 - 16;
		//FocusFree(x + (connection.x_alignment + (connection.x_alignment < 0 ? 1 : 1)) * 16, active_map->height * 32 - 20);
	}
	else if (y >= active_map->height * 32 && active_map->HasConnection(CONNECTION_SOUTH))
	{
		MapConnection connection = active_map->connections[CONNECTION_SOUTH];
		SwitchMap(active_map->connections[CONNECTION_SOUTH].map);
		focus_entity->x = x + (connection.x_alignment + (connection.x_alignment < 0 ? 0 : 0)) * 16;
		focus_entity->y = 0;
		//FocusFree(x + (connection.x_alignment + (connection.x_alignment < 0 ? 1 : 1)) * 16, -16);
	}
	else if (focus_entity->Snapped())
	{
		if (can_warp)
		{
			Warp* w = active_map->GetWarpAt(focus_entity->x / 16, focus_entity->y / 16);
			if (w && w->dest_map != 255)
			{
				current_fade.SetFadeToBlack(active_map->GetPalette());
				current_fade.Start(w);
				can_warp = false;
			}
		}
		else if (current_fade.Done())
		{
			Warp* to = current_fade.GetWarpTo();
			if (to)
			{
				SwitchMap(to->dest_map);
				focus_entity->x = active_map->GetWarp(to->dest_point).x * 16;
				focus_entity->y = active_map->GetWarp(to->dest_point).y * 16;
				current_fade.SetWarpTo(0);
			}
		}
		/*else if (w)
		{
		current_fade.Reset();
		Warp to = *w;
		SwitchMap(to.dest_map);
		focus_entity->x = active_map->GetWarp(to.dest_point).x * 16;
		focus_entity->y = active_map->GetWarp(to.dest_point).y * 16;
		}*/
	}
	else
		can_warp = true;

	Tileset* tex = ResourceCache::GetTileset(active_map->tileset);
	if (tex)
		tex->AnimateTiles();

	for (int i = 0; i < entities.size(); i++)
	{
		if (entities[i])
			entities[i]->Update();
	}

	if (!current_fade.Done() && current_fade.CurrentFade() != current_fade.LastFade())
	{
		SetPalette(current_fade.GetCurrentPalette());
	}
}

void MapScene::Render(sf::RenderWindow* window)
{
	FocusFree(focus_entity->x, focus_entity->y);
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

	focus_entity->Render(window);
	window->setView(window->getDefaultView());

	for (unsigned int i = 0; i < textboxes.size(); i++)
		textboxes[i]->Render(window);
}

void MapScene::SwitchMap(unsigned char index)
{
	ClearEntities();
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

	if (focus_entity)
		focus_entity->SetMap(active_map);

	SetPalette(active_map->GetPalette());

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

void MapScene::ClearEntities(bool focused)
{
	for (int i = 0; i < entities.size(); i++)
	{
		if (entities[i] != focus_entity || focused)
			delete entities[i];
	}
}

void MapScene::SetPalette(sf::Color* pal)
{
	Tileset* tex = ResourceCache::GetTileset(active_map->tileset);

	if (tex)
	{
		tex->SetPalette(pal);
		for (int i = 0; i < 4; i++)
		{
			if (active_map->connected_maps[i])
				ResourceCache::GetTileset(active_map->connected_maps[i]->tileset)->SetPalette(pal);
		}
	}

	ResourceCache::GetMenuTexture()->SetPalette(pal);
	ResourceCache::GetFontTexture()->SetPalette(pal);

	for (int i = 0; i < entities.size(); i++)
	{
		if (entities[i])
			entities[i]->SetPalette(pal);
	}
}