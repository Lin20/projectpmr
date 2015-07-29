#include "BattleScene.h"
#include "BattleConstants.h"

BattleScene::BattleScene()
{
	for (int i = 0; i < 6; i++)
		opponent[i] = 0;
	stage = 0;
	wild_battle = false;
	opponent_image = 0;
	scroll_timer = 0;
}

BattleScene::~BattleScene()
{
	CleanupBattle();
}

void BattleScene::CleanupBattle()
{
	stage = 0;
	scroll_timer = 0;

	for (int i = 0; i < 6; i++)
	{
		if (opponent[i] != 0)
		{
			delete opponent[i];
			opponent[i] = 0;
		}
	}

	if (opponent_image)
	{
		//delete opponent_image;
		//opponent_image = 0;
	}
	if (party_status)
	{
		delete party_status;
		party_status = 0;
	}

	CloseAll(); //close all textboxes
}

void BattleScene::Update()
{
	switch (stage)
	{
	case BattleStages::SCROLL: //0
		UpdateScroll();
		break;
	case BattleStages::INTRO: //1
		UpdateIntro();
		break;
	}
}

void BattleScene::Render(sf::RenderWindow* window)
{
	window->clear(ResourceCache::GetPalette(GRAYSCALE_PALETTE)[0]);
	switch (stage)
	{
	case BattleStages::SCROLL: //0
		RenderScroll(window);
		break;
	case BattleStages::INTRO: //1
		RenderIntro(window);
		break;
	}

	for (unsigned int i = 0; i < textboxes.size(); i++)
		textboxes[i]->Render(window);
}

void BattleScene::BeginWildBattle(unsigned char id, unsigned char level)
{
	wild_battle = 0;
	opponent[0] = new Pokemon(id, level);
	opponent_image = ResourceCache::GetPokemonFront(opponent[0]->pokedex_index);
	InitBattle();
}

void BattleScene::InitBattle()
{
	//set palettes
	sf::Color bw_alpha[4];
	memcpy(bw_alpha, ResourceCache::GetPalette(GRAYSCALE_PALETTE), sizeof(sf::Color) * 4);
	bw_alpha[0] = sf::Color(0, 0, 0, 0);
	ResourceCache::GetRedBack()->SetPalette(bw_alpha);
	opponent_image->SetPalette(bw_alpha);
	ResourceCache::GetMenuTexture()->SetPalette(ResourceCache::GetPalette(GRAYSCALE_PALETTE));
	ResourceCache::GetFontTexture()->SetPalette(ResourceCache::GetPalette(GRAYSCALE_PALETTE));

	status_box = new Textbox();
	textboxes.push_back(status_box);
}

/*
 * Stage 0
 */
void BattleScene::UpdateScroll()
{
	if (scroll_timer < 152)
	{
		scroll_timer += 2;
	}
	else
	{
		stage++;
		ResourceCache::GetRedBack()->SetPalette(ResourceCache::GetPalette(TRAINER_PALETTE));
		opponent_image->SetPalette(ResourceCache::GetPalette(ResourceCache::GetPokemonPaletteIndex(opponent[0]->pokedex_index)));
		status_box->SetText(new TextItem(status_box, nullptr, pokestring("Wild ").append(ResourceCache::GetPokemonName(opponent[0]->id - 1)).append(pokestring("\nappeared!\f"))));
		status_box->SetCloseCallback([this](TextItem* src) { CleanupBattle();  Engine::SwitchState(OVERWORLD); });
		UpdatePartyStatus();
		Engine::GetCryPlayer().Play(opponent[0]->id);
	}
}

void BattleScene::RenderScroll(sf::RenderWindow* window)
{
	sf::Sprite s;
	sf::IntRect rect(0, 0, 0, 0);
	//draw opponent front
	s.setTexture(*opponent_image);
	rect.width = opponent[0]->size_x * 8;
	rect.height = opponent[0]->size_y * 8;
	s.setTextureRect(rect);
	s.setPosition(-opponent[0]->size_x * 8 + scroll_timer - 8, 56 - opponent[0]->size_y * 8);
	window->draw(s);

	//draw player back
	s.setScale(2, 2);
	s.setTexture(*ResourceCache::GetRedBack());
	s.setPosition(160 - scroll_timer, 40);
	window->draw(s);
}

/*
 * Stage 1
 */
void BattleScene::UpdateIntro()
{
	while (!Engine::GetCryPlayer().TrackEnded());
	UpdateTextboxes();
}

void BattleScene::RenderIntro(sf::RenderWindow* window)
{
	sf::Sprite s;
	sf::IntRect rect(0, 0, 0, 0);
	//draw opponent front
	s.setTexture(*opponent_image);
	rect.width = opponent[0]->size_x * 8;
	rect.height = opponent[0]->size_y * 8;
	s.setTextureRect(rect);
	s.setPosition(144 - opponent[0]->size_x * 8, 56 - opponent[0]->size_y * 8);
	window->draw(s);

	//draw player back
	s.setScale(2, 2);
	s.setTexture(*ResourceCache::GetRedBack());
	s.setPosition(8, 40);
	window->draw(s);

	//draw status
	party_status->Render(window, 0, 0, 0, 10, 2, 72, 80);

}

void BattleScene::UpdatePartyStatus()
{
	if (party_status == 0)
	{
		party_status = new TileMap(ResourceCache::GetStatusesTexture(3), 0, 16);
		DataBlock* formation = new DataBlock(20);
		party_status->SetFormation(formation);
	}

	unsigned char* f = party_status->GetFormation()->data_start;
	for (int x = 0; x < 10; x++)
	{
		f[x] = 0x3F; //blank
		f[x + 10] = 0x3F; //blank
		if (x >= 1 && x <= 8)
			f[10 + x] = 0x24; //lower line
	}
	f[10] = 0x20; //arrow
	f[9] = 0x21; //corner bar
	f[19] = 0x25; //right bar

	for (int i = 0; i < 6; i++)
	{
		if (i >= Players::GetPlayer1()->GetPartyCount())
		{
			f[i + 2] = 0x33;
		}
		else if (Players::GetPlayer1()->GetParty()[i]->hp > 0)
		{
			if (Players::GetPlayer1()->GetParty()[i]->status != Statuses::OK)
				f[i + 2] = 0x31;
			else
				f[i + 2] = 0x30;
		}
		else
			f[i + 2] = 0x32;
	}
}
