#pragma once

#include "PlayerProperties.h"

class Players
{
public:
	Players();
	~Players();

	static PlayerProperties* GetPlayer1() { return player1; }
	static PlayerProperties* GetPlayer2() { return player2; }

	static void Initialize();
	static void ReleaseResources();

	static void InitPlayer(PlayerProperties* p);

private:
	static PlayerProperties* player1;
	static PlayerProperties* player2;
};

