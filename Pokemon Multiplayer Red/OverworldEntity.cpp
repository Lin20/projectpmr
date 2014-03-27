#include "OverworldEntity.h"

OverworldEntity::OverworldEntity(Map* m, unsigned char index, unsigned char x, unsigned char y, unsigned char direction, bool npc) : TileMap()
{
	this->on_map = m;
	this->index = index;
	this->x = x * 16;
	this->y = y * 16;
	this->is_npc = npc;

	this->tiles_tex = ResourceCache::GetEntityTexture(index);
	this->formation = new DataBlock(4);
	this->tiles_x = 16;

	step_frame = 0;
	animation_timer = 0;
	movement_direction = 0xFF;
	turn_delay_timer = 0;

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
	if (turn_delay_timer > 0)
		turn_delay_timer--;
	if (animation_timer > 0)
		animation_timer--;

	switch (movement_type)
	{
	case MOVEMENT_NORMAL:
		if (movement_direction != 0xFF || !Snapped() || forced_steps)
		{
			if (step_timer < 1)
			{
				if (!Snapped() || on_map->IsPassable(x / 16 + DELTAX(direction), y / 16 + DELTAY(direction)))
				{
					x += DELTAX(direction);
					y += DELTAY(direction);
				}
				if (Snapped())
				{
					if (steps_remaining > 0)
						steps_remaining--;
					step_timer = STEP_TIMER * 2;
					forced_steps = false;
				}
			}
			if ((int)animation_timer == 0)
			{
				step_frame = (step_frame + 1) % (direction == ENTITY_LEFT || direction == ENTITY_RIGHT ? 2 : 4);
				animation_timer = STEP_TIMER * 8.0f;
			}
		}
		else
		{
			if ((int)animation_timer == 0)
				step_frame = 0;
		}
		break;
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
			if ((step_frame & 1) != 0)
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

void OverworldEntity::StartMoving(unsigned char direction)
{
	if (!Snapped() || !ISNPC(index) || movement_type != MOVEMENT_NORMAL || steps_remaining > 0 || turn_delay_timer > 0 || forced_steps)
		return;

	bool same_dir = this->direction == direction;
	Face(direction);
	if (!same_dir && movement_direction == MOVEMENT_NONE)
	{
		turn_delay_timer = 3.0f;
		return;
	}

	if (animation_timer < 1)
	{
		if (movement_direction == MOVEMENT_NONE)
			animation_timer = STEP_TIMER * 10.0f; //wait 2 extra ticks before actually moving
		else
			animation_timer = STEP_TIMER * 8.0f;
	}

	if (on_map->IsPassable(x / 16 + DELTAX(direction), y / 16 + DELTAY(direction)))
	{
		if (!same_dir)
			step_timer = STEP_TIMER * 2.0f;
		else
		{
			if (step_timer < 1)
				step_timer = STEP_TIMER * 4.0f;
		}
	}

	movement_direction = direction;
}

void OverworldEntity::StopMoving()
{
	if (movement_direction != MOVEMENT_NONE && step_timer > 0 && Snapped())
		forced_steps = true;
	movement_direction = MOVEMENT_NONE;
}

void OverworldEntity::Move(unsigned char direction, unsigned char steps)
{
	if (!Snapped() || !ISNPC(index) || (steps_remaining > 0 && movement_type == MOVEMENT_NORMAL))
		return;

	bool same_dir = this->direction == direction;
	bool reset = step_timer < 1 && !same_dir;
	Face(direction);
	if (is_npc && !on_map->IsPassable(x / 16 + DELTAX(direction), y / 16 + DELTAY(direction)))
	{
		steps_remaining = 0;
		return;
	}
	steps_remaining = steps;
	if (animation_timer < 1)
		animation_timer = STEP_TIMER * 16.0f;
	if (reset && movement_type == MOVEMENT_NORMAL)
	{
		step_timer = 3.0f;
		step_frame = 0;
	}
	else
	{
		if (!same_dir || step_timer > 0)
		{
			step_timer += STEP_TIMER;
			if (on_map->IsPassable(x / 16 + DELTAX(direction), y / 16 + DELTAY(direction)))
			{
				x += DELTAX(direction);
				y += DELTAY(direction);
			}
			movement_type = MOVEMENT_NORMAL;
		}
		else
		{
			steps_remaining++;
			step_timer = STEP_TIMER * 2.0f;
			movement_type = MOVEMENT_NORMAL;
		}
	}
}