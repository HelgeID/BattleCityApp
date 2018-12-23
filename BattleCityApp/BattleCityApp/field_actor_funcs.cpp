#include "field.h"
#include "general.hpp"

void GameField::CreateActors()
{
	firstPlayer = new Actor(texture, true);
	secondPlayer = new Actor(texture, true);

	sf::Vector2f posFP(64.f, 208.f);
	firstPlayer->loadTank(YELLOW, modA, UP);
	firstPlayer->setPosObj(posFP.x, posFP.y);

	sf::Vector2f posSP(192.f, 208.f);
	secondPlayer->loadTank(GREEN, modA, UP);
	secondPlayer->setPosObj(posSP.x, posSP.y);

	return;
}

void GameField::MonitoringKeys()
{
	//sending: UpdateEvent()
	//receiving: MonitoringKeys() - (this)
	if (!KeyActive)
		return;

	if (Key_A == true && (!Key_D && !Key_W && !Key_S)) {
		firstPlayer->moveObj(-1.f, 0.f);
	}
	else if (Key_D == true && (!Key_A && !Key_W && !Key_S)) {
		firstPlayer->moveObj(1.f, 0.f);
	}
	else if (Key_W == true && (!Key_A && !Key_D && !Key_S)) {
		firstPlayer->moveObj(0.f, -1.f);
	}
	else if (Key_S == true && (!Key_A && !Key_D && !Key_W)) {
		firstPlayer->moveObj(0.f, 1.f);
	}

	if (Key_Left == true && (!Key_Right && !Key_Up && !Key_Down)) {
		secondPlayer->moveObj(-1.f, 0.f);
	}
	else if (Key_Right == true && (!Key_Left && !Key_Up && !Key_Down)) {
		secondPlayer->moveObj(1.f, 0.f);
	}
	else if (Key_Up == true && (!Key_Left && !Key_Right && !Key_Down)) {
		secondPlayer->moveObj(0.f, -1.f);
	}
	else if (Key_Down == true && (!Key_Left && !Key_Right && !Key_Up)) {
		secondPlayer->moveObj(0.f, 1.f);
	}

	return;
}