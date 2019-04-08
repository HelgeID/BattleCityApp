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

void RestartPlayer(GameField* gField, const std::string player_name)
{
	sf::sleep(sf::milliseconds(2000));

	mtx.lock();
	player_name == "first player" ? gField->RestartFirstPlayer() 
		: 0;
	player_name == "second player" ? gField->RestartSecondPlayer() 
		: 0;
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
	firstPlayer->numStar() = 0;
	firstPlayer->loadTank(YELLOW, playerModA, UP, false);
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
	secondPlayer->numStar() = 0;
	secondPlayer->loadTank(GREEN, playerModA, UP, false);
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
		bool posLeft = player->getPosObj().x < 32.f && currentDir == Direction::LEFT;
		bool posUp = player->getPosObj().y < 16.f && currentDir == Direction::UP;
		bool posRight = player->getPosObj().x > 224.f && currentDir == Direction::RIGHT;
		bool posDown = player->getPosObj().y > 208.f && currentDir == Direction::DOWN;

		bool f(false);
		f = !f ? [=](const float position) {
			posLeft ? player->setPosObj(position, player->getPosObj().y) : NULL; return posLeft;
		} (32.f) : f;
		f = !f ? [=](const float position) {
			posUp ? player->setPosObj(player->getPosObj().x, position) : NULL; return posUp;
		} (16.f) : f;
		f = !f ? [=](const float position) {
			posRight ? player->setPosObj(position, player->getPosObj().y) : NULL; return posRight;
		} (224.f) : f;
		f = !f ? [=](const float position) {
			posDown ? player->setPosObj(player->getPosObj().x, position) : NULL; return posDown;
		} (208.f) : f;

		f ? player->setPosFrame(player->getPosObj().x, player->getPosObj().y) : NULL;
	}

	if (currentDir == nextDir)
		return;

	if ((currentDir == UP && nextDir == DOWN) || (currentDir == DOWN && nextDir == UP)
		|| (currentDir == LEFT && nextDir == RIGHT) || (currentDir == RIGHT && nextDir == LEFT))
		return;

	const int correct(4); //quarter of the cell size
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

	//std::cerr << "PL1:" << gField.firstPlayer->getPosObj().x << "::" << gField.firstPlayer->getPosObj().y << std::endl;
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

	//std::cerr << "PL2:" << gField.secondPlayer->getPosObj().x << "::" << gField.secondPlayer->getPosObj().y << std::endl;
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

		if (Key_F == true && !firstPlayer->optTankShooting.bulletActivFlag && 
			time_firstPlayer.asSeconds() >= (firstPlayer->numStar() > 0 ? PlayerRechargeTime_Star : PlayerRechargeTime)) {
			CreateBullet(*firstPlayer);
			time_firstPlayer = clock_firstPlayer.restart();
			//{
			//	for (int i(0); i < 8; i++)
			//		bulletArr[i] != nullptr ? std::cerr << bulletArr[i]->indexTank << "-" : std::cerr << "X" << "-";
			//	std::cerr << std::endl;
			//}

			std::unique_ptr<std::thread> thread_snd(new std::thread(&ShootSnd, &sound));
			thread_snd->detach();
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

		if (Key_Ctrl == true && !secondPlayer->optTankShooting.bulletActivFlag && 
			time_secondPlayer.asSeconds() >= (secondPlayer->numStar() > 0 ? PlayerRechargeTime_Star : PlayerRechargeTime)) {
			CreateBullet(*secondPlayer);
			time_secondPlayer = clock_secondPlayer.restart();
			//{
			//	for (int i(0); i < 8; i++)
			//		bulletArr[i] != nullptr ? std::cerr << bulletArr[i]->indexTank << "-" : std::cerr << "X" << "-";
			//	std::cerr << std::endl;
			//}

			std::unique_ptr<std::thread> thread_snd(new std::thread(&ShootSnd, &sound));
			thread_snd->detach();
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
		player.declife();
		std::unique_ptr<std::thread> thread_snd(new std::thread(&Explosion_fSnd, &sound));
		thread_snd->detach();

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

		if (player.takelife() != 0) {
			std::unique_ptr<std::thread> threadPlayer(new std::thread(&RestartPlayer, this, player.name));
			threadPlayer->detach();
		}
	}

exit:
	;
	return;
}

void GameField::PerfectionPlayer(Player& player)
{
	const Color col(player.optTank.col);
	const Direction dir(player.optTank.dir);

	switch (player.numStar())
	{
	case 0: player.loadTank(col, playerModA, dir, false); goto exit;
	case 1: player.loadTank(col, playerModB, dir, false); goto exit;
	case 2: player.loadTank(col, playerModC, dir, false); goto exit;
	case 3: player.loadTank(col, playerModD, dir, false); goto exit;
	}

exit:
	;
	return;
}
