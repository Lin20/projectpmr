#include "OverworldEntity.h"

OverworldEntity::OverworldEntity(Map* m, unsigned char index, unsigned char x, unsigned char y, unsigned char direction) : TileMap()
{
	this->on_map = m;
	this->index = index;
	this->x = x * 16;
	this->y = y * 16;

	this->tiles_tex = ResourceCache::GetEntityTexture(index);
	this->formation = new DataBlock(4);
	this->tiles_x = 16;

	//is this entity a moving npc or a static image (eg. pokeball)
	direction = (direction > 3 ? 0 : index <= ENTITY_LIMIT ? direction : 0);
	for (int i = 0; i < 4; i++)
		formation->data[i] = direction * 4 + i;

	sf::Color palette[4] = { sf::Color(0, 0, 0, 0), *on_map->GetPalette(), on_map->GetPalette()[1], on_map->GetPalette()[3] };
	tiles_tex->SetPalette(palette);
	sprite8x8.setTexture(*tiles_tex);

	Face(direction);
}

OverworldEntity::~OverworldEntity()
{
}

void OverworldEntity::Update()
{
	if (step_timer > 0)
		step_timer--;
	if (steps_remaining > 0)
	{
		if (step_timer < 1)
		{
			if (Snapped())
			{
				steps_remaining--;
				if (steps_remaining > 0)
				{
					step_timer += STEP_TIMER;
					x += DELTAX(direction);
					y += DELTAY(direction);
					if ((DELTAX(direction) != 0 && x % 8 == 0) || (DELTAY(direction) != 0 && y % 8 == 0))
						step_frame = (step_frame + 1) % (direction == ENTITY_LEFT || direction == ENTITY_RIGHT ? 2 : 4);
				}
				else
				{
					step_timer = STEP_TIMER * 2;
				}
			}
			else
			{
				step_timer += STEP_TIMER;
				x += DELTAX(direction);
				y += DELTAY(direction);
				if ((DELTAX(direction) != 0 && x % 8 == 0) || (DELTAY(direction) != 0 && y % 8 == 0))
					step_frame = (step_frame + 1) % (direction == ENTITY_LEFT || direction == ENTITY_RIGHT ? 2 : 4);
			}
		}
	}
}

void OverworldEntity::Render(sf::RenderWindow* window)
{
	if (!tiles_tex)
		return;
	sf::IntRect src_rect = sf::IntRect(0, 0, 8, 8);
	int dest_x = x;
	int dest_y = y;
	bool h_flip = direction == ENTITY_RIGHT;
	for (unsigned int y = 0; y < 2; y++)
	{
		for (unsigned int x = 0; x < 2; x++)
		{
			unsigned char t = (formation ? formation->data[y * 2 + x] : y * 2 + x);
			if (steps_remaining > 0 && (step_frame & 1) != 0)
			{
				t += 12;
				if (direction == ENTITY_DOWN || direction == ENTITY_UP)
					h_flip = (step_frame) > 1;
			}
			src_rect.left = (t % tiles_x) * 8 + (h_flip ? 8 : 0);
			src_rect.top = (t / tiles_x) * 8;
			src_rect.width = (h_flip ? -8 : 8);
			sprite8x8.setTextureRect(src_rect);

			dest_x = (int)(this->x + (h_flip ? 1 - x : x) * 8);
			dest_y = (int)(this->y + y * 8) - 4;
			sprite8x8.setPosition((float)dest_x, (float)dest_y);
			window->draw(sprite8x8);
		}
	}
}

void OverworldEntity::Face(unsigned char direction)
{
	if (!Snapped() || !ISNPC(index))
		return;
	unsigned char f = direction * 4;
	if (direction == ENTITY_RIGHT)
		f = ENTITY_LEFT * 4;
	for (int i = 0; i < 4; i++)
		formation->data[i] = f++;
	this->direction = direction;
}

void OverworldEntity::Move(unsigned char direction, unsigned char steps)
{
	if (!Snapped() || !ISNPC(index) || steps_remaining > 0)
		return;

	bool same_dir = this->direction == direction;
	bool reset = step_timer < 1 && !same_dir;
	Face(direction);
	steps_remaining = steps;
	if (reset)
	{
		step_timer = 3.0f;
		step_frame = 0;
	}
	else
	{
		if (!same_dir || step_timer > 0)
		{
			step_timer += STEP_TIMER;
			x += DELTAX(direction);
			y += DELTAY(direction);
		}
		else
		{
			step_timer = STEP_TIMER * 2.0f;
			steps_remaining++;
		}
	}
}