#include "field.h"
#include "general.hpp"

#define speedActor (1.f)

void GameField::CreateActors()
{
	firstPlayer = new Actor(texture, true);
	secondPlayer = new Actor(texture, true);

	sf::Vector2f posFP(64.f, 208.f);
	firstPlayer->loadTank(YELLOW, modA, UP);
	firstPlayer->setPosObj(posFP.x, posFP.y);
	firstPlayer->loadIndex(100); // One hundred - just a constant variable for firstPlayer

	sf::Vector2f posSP(192.f, 208.f);
	secondPlayer->loadTank(GREEN, modA, UP);
	secondPlayer->setPosObj(posSP.x, posSP.y);
	secondPlayer->loadIndex(200); // Two hundred - just a constant variable for secondPlayer

	return;
}

void MoveFirstPlayer(GameField& gField, const Direction nextDir)
{
	float posX(0.f), posY(0.f);
	const Direction currentDir = gField.firstPlayer->optTank.dir;
	switch (currentDir)
	{
	case UP: posY = -speedActor; break;
	case LEFT: posX = -speedActor; break;
	case DOWN: posY = speedActor; break;
	case RIGHT: posX = speedActor; break;
	}
	currentDir != nextDir ?
		gField.firstPlayer->loadTank(gField.firstPlayer->optTank.col, gField.firstPlayer->optTank.mod, nextDir) :
		gField.firstPlayer->moveObj(posX, posY);
	return;
}

void MoveSecondPlayer(GameField& gField, const Direction nextDir)
{
	float posX(0.f), posY(0.f);
	const Direction currentDir = gField.secondPlayer->optTank.dir;
	switch (currentDir)
	{
	case UP: posY = -speedActor; break;
	case LEFT: posX = -speedActor; break;
	case DOWN: posY = speedActor; break;
	case RIGHT: posX = speedActor; break;
	}
	currentDir != nextDir ?
		gField.secondPlayer->loadTank(gField.secondPlayer->optTank.col, gField.secondPlayer->optTank.mod, nextDir) :
		gField.secondPlayer->moveObj(posX, posY);
	return;
}

void GameField::MonitoringKeys()
{
	//sending: UpdateEvent()
	//receiving: MonitoringKeys() - (this)
	if (!KeyActive)
		return;

	//A-D-W-S
	if (Key_A == true && (!Key_D && !Key_W && !Key_S)) {
		MoveFirstPlayer(*this, LEFT);
	}
	else if (Key_D == true && (!Key_A && !Key_W && !Key_S)) {
		MoveFirstPlayer(*this, RIGHT);
	}
	else if (Key_W == true && (!Key_A && !Key_D && !Key_S)) {
		MoveFirstPlayer(*this, UP);
	}
	else if (Key_S == true && (!Key_A && !Key_D && !Key_W)) {
		MoveFirstPlayer(*this, DOWN);
	}

	//LEFT-RIGHT-UP-DOWN
	if (Key_Left == true && (!Key_Right && !Key_Up && !Key_Down)) {
		MoveSecondPlayer(*this, LEFT);
	}
	else if (Key_Right == true && (!Key_Left && !Key_Up && !Key_Down)) {
		MoveSecondPlayer(*this, RIGHT);
	}
	else if (Key_Up == true && (!Key_Left && !Key_Right && !Key_Down)) {
		MoveSecondPlayer(*this, UP);
	}
	else if (Key_Down == true && (!Key_Left && !Key_Right && !Key_Up)) {
		MoveSecondPlayer(*this, DOWN);
	}

	if (Key_F == true && !firstPlayer->optTankShooting.bulletActivFlag) {
		CreateBullet(*firstPlayer);
		for (int i(0); i < 6; i++)
			bulletArr[i] != nullptr ? std::cout << bulletArr[i]->indexTank << "-" : std::cout << "X" << "-";
		std::cout << std::endl;
	}
		

	if (Key_Ctrl == true && !secondPlayer->optTankShooting.bulletActivFlag) {
		CreateBullet(*secondPlayer);
		for (int i(0); i < 6; i++)
			bulletArr[i] != nullptr ? std::cout << bulletArr[i]->indexTank << "-" : std::cout << "X" << "-";
		std::cout << std::endl;
	}
		
	return;
}

auto boom = [](Tank& tank1, Tank& tank2)
{
	sf::FloatRect bulletRectPlayer1 = tank1.takeObj().getGlobalBounds();
	sf::FloatRect bulletRectPlayer2 = tank2.takeObj().getGlobalBounds();

	const bool fX(bulletRectPlayer1.left > bulletRectPlayer2.left);
	const bool fY(bulletRectPlayer1.top > bulletRectPlayer2.top);

	if (tank2.optTank.dir == UP) {
		if (tank1.optTank.dir == UP) {
			fY == true ? tank1.moveObj(0.f, 1.f) :
				tank2.moveObj(0.f, 1.f);
		}
		else if (tank1.optTank.dir == LEFT) {
			fX == true ? tank1.moveObj(1.f, 0.f) :
				!fY ? tank2.moveObj(0.f, 1.f) : NULL;
		}
		else if (tank1.optTank.dir == DOWN) {
			!fY ? tank1.moveObj(0.f, -1.f),
				tank2.moveObj(0.f, 1.f) : NULL;
		}
		else if (tank1.optTank.dir == RIGHT) {
			!fX ? tank1.moveObj(-1.f, 0.f) :
				!fY ? tank2.moveObj(0.f, 1.f) : NULL;
		}
	}
	else if (tank2.optTank.dir == LEFT) {
		if (tank1.optTank.dir == UP) {
			fY == true ? tank1.moveObj(0.f, 1.f) :
				!fX ? tank2.moveObj(1.f, 0.f) : NULL;
		}
		else if (tank1.optTank.dir == LEFT) {
			fX == true ? tank1.moveObj(1.f, 0.f) :
				tank2.moveObj(1.f, 0.f);
		}
		else if (tank1.optTank.dir == DOWN) {
			!fY ? tank1.moveObj(0.f, -1.f) :
				!fX ? tank2.moveObj(1.f, 0.f) : NULL;
		}
		else if (tank1.optTank.dir == RIGHT) {
			!fX ? tank1.moveObj(-1.f, 0.f),
				tank2.moveObj(1.f, 0.f) : NULL;
		}
	}
	else if (tank2.optTank.dir == DOWN) {
		if (tank1.optTank.dir == UP) {
			fY == true ? tank1.moveObj(0.f, 1.f),
				tank2.moveObj(0.f, -1.f) : NULL;
		}
		else if (tank1.optTank.dir == LEFT) {
			fX == true ? tank1.moveObj(1.f, 0.f) :
				fY == true ? tank2.moveObj(0.f, -1.f) : NULL;
		}
		else if (tank1.optTank.dir == DOWN) {
			!fY ? tank1.moveObj(0.f, -1.f) :
				tank2.moveObj(0.f, -1.f);
		}
		else if (tank1.optTank.dir == RIGHT) {
			!fX ? tank1.moveObj(-1.f, 0.f) :
				fY == true ? tank2.moveObj(0.f, -1.f) : NULL;
		}
	}
	else if (tank2.optTank.dir == RIGHT) {
		if (tank1.optTank.dir == UP) {
			fY == true ? tank1.moveObj(0.f, 1.f) :
				fX == true ? tank2.moveObj(-1.f, 0.f) : NULL;
		}
		else if (tank1.optTank.dir == LEFT) {
			fX == true ? tank1.moveObj(1.f, 0.f),
				tank2.moveObj(-1.f, 0.f) : NULL;
		}
		else if (tank1.optTank.dir == DOWN) {
			!fY ? tank1.moveObj(0.f, -1.f) :
				fX == true ? tank2.moveObj(-1.f, 0.f) : NULL;
		}
		else if (tank1.optTank.dir == RIGHT) {
			!fX ? tank1.moveObj(-1.f, 0.f) :
				tank2.moveObj(-1.f, 0.f);
		}
	}
};

void GameField::CHECK_ACTOR_ON_COLLISION_ACTOR(Actor *player1, Actor *player2)
{
	if (player1->takeObj().getGlobalBounds().intersects(player2->takeObj().getGlobalBounds()))
	{
		if (Key_A || Key_D || Key_W || Key_S)
			boom(*player1, *player2);
		if (Key_Left || Key_Right || Key_Up || Key_Down)
			boom(*player2, *player1);
	}
}

void GameField::CHECK_ACTOR_ON_COLLISION_BLOCKS(Actor *player1, Actor *player2)
{

}

void GameField::CHECK_ACTOR_ON_COLLISION_BULLET(Actor *player1, Actor *player2)
{

}

void GameField::CHECK_ACTOR_ON_COLLISION_ENEMIES(Actor *player1, Actor *player2)
{

}

void GameField::CHECK_ACTOR_ON_COLLISION_FRAME(Actor *player1, Actor *player2)
{
	auto collisionframe = [&](Actor *player)
	{
		if (player->takeObj().getGlobalBounds().intersects(outsideUP.getGlobalBounds()))
			player->moveObj(0.f, 1.f);
		else if (player->takeObj().getGlobalBounds().intersects(outsideDOWN.getGlobalBounds()))
			player->moveObj(0.f, -1.f);
		else if (player->takeObj().getGlobalBounds().intersects(outsideLEFT.getGlobalBounds()))
			player->moveObj(1.f, 0.f);
		else if (player->takeObj().getGlobalBounds().intersects(outsideRIGHT.getGlobalBounds()))
			player->moveObj(-1.f, 0.f);
	};

	if (player1->Presence())
		collisionframe(player1);

	if (player2->Presence())
		collisionframe(player2);

	return;
}
