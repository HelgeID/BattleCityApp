#include <thread>
#include "field.h"
#include "general.hpp"

struct AnimArgPtr { Player* player; AnimBirth* animBirth; };
void AnimBirthPlayer(AnimArgPtr argPtr)
{
	while (!argPtr.animBirth->FinishTime())
		;
	argPtr.player->Presence() = true;
	argPtr.player->SkinOn();
	return;
}

void GameField::CreateActors()
{
	firstPlayer = new Player(texture, "first player", false);
	secondPlayer = new Player(texture, "second player", false);
	RestartFirstPlayer();
	RestartSecondPlayer();
	return;
}

void GameField::RestartFirstPlayer(const bool flag)
{
	//sf::sleep(sf::milliseconds(500)); //test sleep
	sf::Vector2f posFirstPlayer(64.f, 208.f);
	firstPlayer->loadTank(YELLOW, modA, UP);
	firstPlayer->setPosObj(posFirstPlayer.x, posFirstPlayer.y);
	firstPlayer->loadIndex(100); // One hundred - just a constant variable for firstPlayer

	if (flag == true)
	{
		!firstPlayer->Presence() ? firstPlayer->Presence() = true : NULL;
		return;
	}

	std::unique_ptr<AnimBirth> anim(new AnimBirth(texture, posFirstPlayer));
	firstPlayerAnim.playerBirth = std::move(anim);
	AnimArgPtr argPtr{ firstPlayer, firstPlayerAnim.playerBirth.get() };

	std::unique_ptr<std::thread> threadPlayer(new std::thread(&AnimBirthPlayer, argPtr));
	threadPlayer->detach();
	return;
}

void GameField::RestartSecondPlayer(const bool flag)
{
	//sf::sleep(sf::milliseconds(500)); //test sleep
	sf::Vector2f posSecondPlayer(192.f, 208.f);
	secondPlayer->loadTank(GREEN, modA, UP);
	secondPlayer->setPosObj(posSecondPlayer.x, posSecondPlayer.y);
	secondPlayer->loadIndex(200); // Two hundred - just a constant variable for secondPlayer

	if (flag == true)
	{
		!secondPlayer->Presence() ? secondPlayer->Presence() = true : NULL;
		return;
	}

	std::unique_ptr<AnimBirth> anim(new AnimBirth(texture, posSecondPlayer));
	secondPlayerAnim.playerBirth = std::move(anim);
	AnimArgPtr argPtr{ secondPlayer, secondPlayerAnim.playerBirth.get() };

	std::unique_ptr<std::thread> threadPlayer(new std::thread(&AnimBirthPlayer, argPtr));
	threadPlayer->detach();
	return;
}

void MoveFirstPlayer(GameField& gField, const Direction nextDir)
{
	float posX(0.f), posY(0.f);
	const Direction currentDir = gField.firstPlayer->optTank.dir;
	switch (currentDir)
	{
	case UP: posY = -PlayerSpeed; break;
	case LEFT: posX = -PlayerSpeed; break;
	case DOWN: posY = PlayerSpeed; break;
	case RIGHT: posX = PlayerSpeed; break;
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
	case UP: posY = -PlayerSpeed; break;
	case LEFT: posX = -PlayerSpeed; break;
	case DOWN: posY = PlayerSpeed; break;
	case RIGHT: posX = PlayerSpeed; break;
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

	if (firstPlayer->Presence()) {
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

		if (Key_F == true && !firstPlayer->optTankShooting.bulletActivFlag && time_firstPlayer.asSeconds() >= PlayerRechargeTime) {
			CreateBullet(*firstPlayer);
			time_firstPlayer = clock_firstPlayer.restart();
			for (int i(0); i < 6; i++)
				bulletArr[i] != nullptr ? std::cout << bulletArr[i]->indexTank << "-" : std::cout << "X" << "-";
			std::cout << std::endl;
		}
	}

	if (secondPlayer->Presence()) {
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

		if (Key_Ctrl == true && !secondPlayer->optTankShooting.bulletActivFlag && time_secondPlayer.asSeconds() >= PlayerRechargeTime) {
			CreateBullet(*secondPlayer);
			time_secondPlayer = clock_secondPlayer.restart();
			for (int i(0); i < 6; i++)
				bulletArr[i] != nullptr ? std::cout << bulletArr[i]->indexTank << "-" : std::cout << "X" << "-";
			std::cout << std::endl;
		}
	}

	return;
}

void GameField::CheckPlayerBang(Player& player)
{
	if (player.GetSkin() || undying_players)
		goto exit;

	//off player
	if (player.Presence()) {
		player.Presence() = false;
		const sf::Vector2f point = player.getPosObj();
		CreateAnimBoom(point, "tankObj");
	}

exit:
	;
	return;
}
