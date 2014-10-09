#include "OverworldEntity.h"
#include "Engine.h"

OverworldEntity::OverworldEntity(Map* m, unsigned char index, unsigned char sprite, unsigned char x, unsigned char y, unsigned char direction, bool npc, Script* _script, std::function<void()> step_callback) : TileMap()
{
	this->on_map = m;
	this->index = index;
	this->sprite = sprite;
	this->x = x * 16;
	this->y = y * 16;
	this->is_npc = npc;
	this->frozen = false;
	this->delete_texture = false;
	this->script = _script;
	this->temp_script = 0;
	this->script_enabled = false;
	if (step_callback)
		this->step_callback = step_callback;
	else
		this->step_callback = nullptr;

	SetSprite(sprite);

	step_frame = 0;
	animation_timer = 0;
	movement_direction = 0xFF;
	turn_delay_timer = 0;
	steps_remaining = 0;
	movement_type = 0;
	forced_steps = false;
	allow_entity_ghosting = false;
	emotion_bubble = 255;
	force_fast = false;
	offset_y = 0;

	//is this entity a moving npc or a static image (eg. pokeball)
	direction = (direction > 3 ? 0 : index <= ENTITY_LIMIT ? direction : 0);
	for (int i = 0; i < 4; i++)
		formation->data[i] = direction * 4 + i;

	DataBlock* emotion_formation = new DataBlock(12);
	for (int i = 0; i < 12; i++)
		emotion_formation->data[i] = i;
	emotion_texture = new TileMap(ResourceCache::GetEmotionBubbles(), emotion_formation, 12);

	if (m)
	{
		sf::Color palette[4] = { sf::Color(0, 0, 0, 0), *on_map->GetPalette(), on_map->GetPalette()[1], on_map->GetPalette()[3] };
		tiles_tex->SetPalette(palette);
		if (!npc)
			ResourceCache::GetShadowTexture()->SetPalette(palette);

		emotion_texture->GetTexture()->SetPalette(palette);
	}
	sprite8x8.setTexture(*tiles_tex);
	shadow8x8.setTexture(*ResourceCache::GetShadowTexture());

	Face(direction);
}

OverworldEntity::~OverworldEntity()
{
	if (script)
		delete script;
	if (temp_script)
		delete temp_script;
	if (emotion_texture)
		delete emotion_texture;
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
		if (movement_direction != 0xFF || !Snapped() || forced_steps || steps_remaining > 0)
		{
			if (step_timer < 1)
			{
				if (!Snapped() || on_map->IsPassable(x / 16 + DELTAX(direction), y / 16 + DELTAY(direction), this, !allow_entity_ghosting))
				{
					x += DELTAX(direction) * (sprite > 0 ? 1 : 2);
					y += DELTAY(direction) * (sprite > 0 ? 1 : 2);
				}
				if (Snapped())
				{
					if (steps_remaining > 0)
					{
						steps_remaining--;
						if (steps_remaining > 0)
						{
							StartMoving(direction);
						}
						else
						{
							if (is_npc)
								StopMoving();
							force_fast = false;
						}
					}
					if (step_callback)
						step_callback();
					step_timer = STEP_TIMER * 2;
					forced_steps = false;
				}
				else if ((is_npc || allow_entity_ghosting) && steps_remaining > 0 && !force_fast)
				{
					step_timer = STEP_TIMER * 2.0f;
				}
			}
			if (!is_npc && Snapped() && !on_map->IsPassable(x / 16 + DELTAX(direction), y / 16 + DELTAY(direction), this, !allow_entity_ghosting))
			{
				if ((int)animation_timer == 0 && step_frame % 2 == 0)
					Engine::GetWorldSounds().Play(SFX_COLLISION);
			}
			if ((int)animation_timer == 0)
			{
				step_frame = (step_frame + 1) % (direction == ENTITY_LEFT || direction == ENTITY_RIGHT ? 2 : 4);
				animation_timer = STEP_TIMER * 8.0f;
			}
		}
		break;

	case MOVEMENT_JUMP:
		//from here to the next step_timer check is all for the slight delay before and after a jump. annoying huh?
		if (jump_index > 250)
		{
			step_timer = 2.0f;
			jump_index++;
			break;
		}

		x += DELTAX(direction);
		y += DELTAY(direction);

		if (step_timer < 1)
		{
			jump_index++;
			if (jump_index == JUMP_STEPS)
			{
				movement_type = MOVEMENT_NORMAL;
				if (steps_remaining > 0)
					steps_remaining--;
				if (steps_remaining == 0 && is_npc)
					StopMoving();
			}
			else
			{
				step_timer = 2.0f;
			}
			if ((int)animation_timer == 0)
			{
				step_frame = (step_frame + 1) % (direction == ENTITY_LEFT || direction == ENTITY_RIGHT ? 2 : 4);
				animation_timer = STEP_TIMER * 8.0f;
			}
		}
		break;
	}

	if ((int)animation_timer == 0)
		step_frame = 0;

	if (script && script_enabled && Snapped() && steps_remaining == 0)
	{
		script->SetEntityIndex(index);
		script->Update();
		if (script->Done())
		{
			SetScriptState(false);
			script->Reset();
		}
	}

	if (temp_script && Snapped() && steps_remaining == 0)
	{
		temp_script->SetEntityIndex(index);
		temp_script->Update();
		if (temp_script->Done())
		{
			delete temp_script;
			temp_script = 0;
		}
	}
}

void OverworldEntity::Render(sf::RenderWindow* window, int offset_x, int offset_y)
{
	if (!tiles_tex)
		return;

	//draw the shadow if jumping
	sf::IntRect src_rect = sf::IntRect(0, 0, 8, 8);
	if (movement_type == MOVEMENT_JUMP)
	{
		for (int i = 0; i < 4; i++)
		{
			int x = this->x + (i % 2) * 8;
			int y = this->y + (i / 2) * 8 + 4;
			src_rect.left = (i % 2) * 8;
			src_rect.top = (i / 2) * 8;
			src_rect.width = 8 + (i % 2) * -16;
			src_rect.height = 8 + (i / 2) * -16;
			shadow8x8.setTextureRect(src_rect);
			shadow8x8.setPosition((float)x, (float)y);
			window->draw(shadow8x8);
		}
	}

	//draw the player
	src_rect.height = 8;
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
			if (movement_type == MOVEMENT_JUMP)
			{
				dest_x = (int)(this->x + (h_flip ? 1 - x : x) * 8);
				int offset = (ResourceCache::GetJumpCoordinates()->data[min(JUMP_STEPS - 3, max(0, (int)(signed char)jump_index))] - 0x3C) - (jump_y - this->y) - 2;
				dest_y = (int)(this->jump_y + y * 8) + offset;
			}
			sprite8x8.setPosition((float)(dest_x + offset_x), (float)(dest_y + offset_y + this->offset_y));
			window->draw(sprite8x8);
		}
	}

	//draw the grass on top of the player... gah!
	//i'll be incredibly relieved when this is over.
	if (on_map && on_map->InGrass(this->x / 16, this->y / 16))
	{
		//store the previous texture and set it to the transparent tiles
		const sf::Texture* tex = sprite8x8.getTexture();
		sprite8x8.setTexture(*ResourceCache::GetTileset(on_map->tileset)->GetTransparentTiles());

		//after several tries with using single grass tiles and drawing things manually i decided to create
		//a function that renders all the tiles in a certain spot
		//much easier...
		on_map->RenderRectangle(this->x, this->y + 4, 16, 8, sprite8x8, window);

		//set the texture back to what it was
		sprite8x8.setTexture(*tex);
	}

	if (emotion_bubble < 3)
	{
		emotion_texture->Render(window, this->x / 16, this->y / 16 - 1, emotion_bubble, 2, 2, 0, -4);
	}
}

void OverworldEntity::Face(unsigned char direction)
{
	if (!Snapped() || !ISNPC(sprite) || direction == MOVEMENT_NONE)
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
	if (!Snapped() || !ISNPC(sprite) || movement_type != MOVEMENT_NORMAL || steps_remaining > 0 || turn_delay_timer > 0 || forced_steps)
		return;

	bool same_dir = this->direction == direction;
	Face(direction);
	//set a delay before the player can move so they can tap the direction to just turn
	if (!same_dir && movement_direction == MOVEMENT_NONE)
	{
		turn_delay_timer = 3.0f;
		return;
	}

	//continue or start the animation cycle
	if (animation_timer < 1)
	{
		if (movement_direction == MOVEMENT_NONE)
			animation_timer = STEP_TIMER * 10.0f; //wait 2 extra ticks before actually moving
		else
			animation_timer = STEP_TIMER * 8.0f;
	}

	//set to walk on update
	if (on_map->IsPassable(x / 16 + DELTAX(direction), y / 16 + DELTAY(direction), this))
	{
		if (!same_dir)
			step_timer = STEP_TIMER * 2.0f;
		else
		{
			if (step_timer < 1)
				step_timer = STEP_TIMER * 4.0f;
		}
	}
	else if (on_map->CanJump(x / 16, y / 16, direction)) //set to jump over a ledge if possible
	{
		movement_type = MOVEMENT_JUMP;
		jump_index = -2;
		jump_x = x;
		jump_y = y;
		step_timer = STEP_TIMER * 2.0f;
		animation_timer = STEP_TIMER * 10.0f; //wait 2 extra ticks before actually moving
		if (!is_npc)
			Engine::GetWorldSounds().Play(SFX_LEDGE);
	}

	movement_direction = direction;
}

void OverworldEntity::StopMoving()
{
	if (movement_direction == MOVEMENT_NORMAL && step_timer > 0 && Snapped())
		forced_steps = true;
	movement_direction = MOVEMENT_NONE;
}

void OverworldEntity::ForceStop()
{
	movement_direction = MOVEMENT_NONE;
	step_timer = 0;
	steps_remaining = 0;
	step_frame = 0;
}

void OverworldEntity::Move(unsigned char direction, unsigned char steps, bool fast)
{
	if (direction == MOVEMENT_NONE)
		return;
	force_fast = fast;
	StartMoving(direction);
	steps_remaining = steps;
	if (is_npc && steps_remaining == 0)
		ForceStop();
}

void OverworldEntity::SetSprite(unsigned char index)
{
	this->sprite = index;
	this->tiles_tex = ResourceCache::GetEntityTexture(index);

	if (!this->formation)
		this->formation = new DataBlock(4);
	this->tiles_x = 16;

	direction = (direction > 3 ? 0 : index <= ENTITY_LIMIT ? direction : 0);
	for (int i = 0; i < 4; i++)
		formation->data[i] = direction * 4 + i;

	if (on_map)
	{
		sf::Color palette[4] = { sf::Color(0, 0, 0, 0), *on_map->GetPalette(), on_map->GetPalette()[1], on_map->GetPalette()[3] };
		tiles_tex->SetPalette(palette);
	}
	sprite8x8.setTexture(*tiles_tex);
	Face(direction);
}

void OverworldEntity::ExecuteScript(Script* script)
{
	temp_script = script;
}
