#include <thread>
#include "field.h"
#include "general.hpp"

struct AnimArgPtr { Player* player; AnimBirth* animBirth; };
void AnimBirthPlayer(AnimArgPtr argPtr)
{
	if (argPtr.animBirth == nullptr)
		return;

	sf::sleep(sf::milliseconds(3000)); //3s -> because AnimBirth 4s
	while (!argPtr.animBirth->FinishTime())
		;

	mtx.lock();
	argPtr.player->Presence() = true;
	argPtr.player->SkinOn();
	mtx.unlock();

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
	sf::Vector2f posFirstPlayer(64.f, 208.f);
	firstPlayer->loadTank(YELLOW, modA, UP, false);
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
	sf::Vector2f posSecondPlayer(192.f, 208.f);
	secondPlayer->loadTank(GREEN, modA, UP, false);
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

void CorrectPosition(Player* player, const Direction currentDir, const Direction nextDir)
{
	{
		if (player->getPosObj().x < 32.f && currentDir == LEFT) {
			player->setPosObj(32.f, player->getPosObj().y);
			player->setPosFrame(player->getPosObj().x, player->getPosObj().y);
			return;
		}

		if (player->getPosObj().y < 16.f && currentDir == UP) {
			player->setPosObj(player->getPosObj().x, 16.f);
			player->setPosFrame(player->getPosObj().x, player->getPosObj().y);
			return;
		}

		if (player->getPosObj().x > 224.f && currentDir == RIGHT) {
			player->setPosObj(224.f, player->getPosObj().y);
			player->setPosFrame(player->getPosObj().x, player->getPosObj().y);
			return;
		}

		if (player->getPosObj().y > 208.f && currentDir == DOWN) {
			player->setPosObj(player->getPosObj().x, 208.f);
			player->setPosFrame(player->getPosObj().x, player->getPosObj().y);
			return;
		}
	}

	if (currentDir == nextDir)
		return;

	if ((currentDir == UP && nextDir == DOWN) || (currentDir == DOWN && nextDir == UP)
		|| (currentDir == LEFT && nextDir == RIGHT) || (currentDir == RIGHT && nextDir == LEFT))
		return;

	const int correct(8); //half of the cell size
	int posX_Correct, posY_Correct;
	currentDir == UP || currentDir == LEFT ? posX_Correct = (int)player->getPosObj().x / correct * correct : NULL;
	currentDir == UP || currentDir == LEFT ? posY_Correct = (int)player->getPosObj().y / correct * correct : NULL;
	currentDir == DOWN || currentDir == RIGHT ? posX_Correct = ((int)player->getPosObj().x + correct - 1) / correct * correct : NULL;
	currentDir == DOWN || currentDir == RIGHT ? posY_Correct = ((int)player->getPosObj().y + correct - 1) / correct * correct : NULL;

	switch (nextDir)
	{
	case UP: case DOWN:
		player->setPosObj((float)posX_Correct, player->getPosObj().y); break;
	case LEFT: case RIGHT:
		player->setPosObj(player->getPosObj().x, (float)posY_Correct); break;
	}

	player->setPosFrame(player->getPosObj().x, player->getPosObj().y);
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

	CorrectPosition(gField.firstPlayer, currentDir, nextDir);

	currentDir != nextDir ?
		gField.firstPlayer->loadTank(gField.firstPlayer->optTank.col, gField.firstPlayer->optTank.mod, nextDir, gField.firstPlayer->optTank.bonus) :
		gField.firstPlayer->moveObj(posX, posY);

	std::cerr << "PL1:" << gField.firstPlayer->getPosObj().x << "::" << gField.firstPlayer->getPosObj().y << std::endl;
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

	CorrectPosition(gField.secondPlayer, currentDir, nextDir);

	currentDir != nextDir ?
		gField.secondPlayer->loadTank(gField.secondPlayer->optTank.col, gField.secondPlayer->optTank.mod, nextDir, gField.secondPlayer->optTank.bonus) :
		gField.secondPlayer->moveObj(posX, posY);

	std::cerr << "PL2:" << gField.secondPlayer->getPosObj().x << "::" << gField.secondPlayer->getPosObj().y << std::endl;
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
			//{
			//	for (int i(0); i < 8; i++)
			//		bulletArr[i] != nullptr ? std::cerr << bulletArr[i]->indexTank << "-" : std::cerr << "X" << "-";
			//	std::cerr << std::endl;
			//}
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
			//{
			//	for (int i(0); i < 8; i++)
			//		bulletArr[i] != nullptr ? std::cerr << bulletArr[i]->indexTank << "-" : std::cerr << "X" << "-";
			//	std::cerr << std::endl;
			//}
		}
	}

	return;
}

void GameField::CheckPlayerBang(Player& player, const bool off)
{
	if ((player.GetSkin() && !off) || undying_players)
		goto exit;

	//off player
	if (player.Presence()) {
		player.Presence() = false;
		const sf::Vector2f point = player.getPosObj();
		CreateAnimBoom(point, "tankObj");

		if (player.name == "first player" && firstPlayerAnim.playerSkin != nullptr)
		{
			firstPlayerAnim.playerSkin.reset();
			firstPlayerAnim.playerSkin = nullptr;
			player.SkinOff();
		}

		if (player.name == "second player" && secondPlayerAnim.playerSkin != nullptr)
		{
			secondPlayerAnim.playerSkin.reset();
			secondPlayerAnim.playerSkin = nullptr;
			player.SkinOff();
		}
	}

exit:
	;
	return;
}
