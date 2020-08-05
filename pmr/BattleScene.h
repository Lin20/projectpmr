#pragma once

#include "Common.h"
#include "Constants.h"
#include "Scene.h"
#include "Textbox.h"
#include "MenuCache.h"
#include "ItemStorage.h"
#include "AudioConstants.h"
#include "Pokemon.h"

class BattleScene : public Scene
{
public:
	BattleScene();
	~BattleScene();
	void CleanupBattle();

	virtual void Update() override;
	virtual void Render(sf::RenderWindow* window) override;

	void BeginWildBattle(unsigned char id, unsigned char level);
	//void EndBattle();

private:
	bool wild_battle;
	Pokemon* opponent[6];
	unsigned char stage;
	PaletteTexture* opponent_image;
	Textbox* status_box;

	void InitBattle();

	/*
	 *STAGE 0 - scroll red/opponent
	 */
	void UpdateScroll();
	void RenderScroll(sf::RenderWindow* window);
	unsigned char scroll_timer;

	/*
	*STAGE 1 - intro [x appeared or x would like to battle]
	*/
	TileMap* party_status; //the pokeballs displaying our party
	void UpdateIntro();
	void RenderIntro(sf::RenderWindow* window);
	void UpdatePartyStatus();
};
