#include "Players.h"

PlayerProperties* Players::player1 = 0;
PlayerProperties* Players::player2 = 0;

Players::Players()
{
}

Players::~Players()
{

}

void Players::Initialize()
{
	player1 = new PlayerProperties();
	player2 = new PlayerProperties();

	InitPlayer(player1);
	InitPlayer(player2);
}

void Players::ReleaseResources()
{
	if (player1)
	{
		delete player1->GetInventory();
		delete player1;
	}
	if (player2)
	{
		delete player2->GetInventory();
		delete player2;
	}
}

void Players::InitPlayer(PlayerProperties* p)
{
	p->SetInventory(new ItemStorage(p));
}