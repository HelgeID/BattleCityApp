#include <thread>
#include "field.h"
#include "general.hpp"

struct AnimArgPtr { Player* player; AnimBirth* animBirth; };
void AnimBirthPlayer(AnimArgPtr argPtr)
{
	if (argPtr.animBirth == nullptr)
		return;

	sf::sleep(sf::milliseconds(1700)); //1.7s -> because AnimBirth 2s
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
	//player_name - player, who must restart
	sf::sleep(sf::milliseconds(1000));
	while (true)
	{
		sf::sleep(sf::milliseconds(1000));
		if (gField->tank.size() == 0)
			break;

		bool collisionF(false);
		bool u1(false), u2(false);
		std::for_each(gField->tank.begin(), gField->tank.end(), [&](Tank &tank)
		{
			//is the tank on the spawnpoint of the player
			u1 = ((tank.isTank() && player_name == "first player") && (tank.takeObj().getGlobalBounds().intersects(gField->lPlayer_BS->takeObj().getGlobalBounds())));
			u2 = ((tank.isTank() && player_name == "second player") && (tank.takeObj().getGlobalBounds().intersects(gField->rPlayer_BS->takeObj().getGlobalBounds())));
			if (u1 || u2)
				collisionF = true; //YES
		});

		//is the player on the spawnpoint of the other player
		(gField->firstPlayer->takeObj().getGlobalBounds().intersects(gField->rPlayer_BS->takeObj().getGlobalBounds()) && player_name == "second player") ||
			(gField->secondPlayer->takeObj().getGlobalBounds().intersects(gField->lPlayer_BS->takeObj().getGlobalBounds()) && player_name == "first player") ?
				collisionF = true : 0;

		if (!collisionF) //NO ->> exit of checks
			break;
	}

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
	TwoPlayer() ? RestartFirstPlayer(), RestartSecondPlayer() : RestartFirstPlayer();
	return;
}

void GameField::RestartFirstPlayer(const bool flag)
{
	sf::Vector2f posFirstPlayer(lPlayer_BS->getPosObj().x, lPlayer_BS->getPosObj().y);
	firstPlayer->numStar() = 0; //NULL by default
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

	std::unique_ptr<std::thread> threadPlayerL(new std::thread(&AnimBirthPlayer, argPtr));
	threadPlayerL->detach();
	lPlayer_BS->Spawn() = true;
	return;
}

void GameField::RestartSecondPlayer(const bool flag)
{
	sf::Vector2f posSecondPlayer(rPlayer_BS->getPosObj().x, rPlayer_BS->getPosObj().y);
	secondPlayer->numStar() = 0; //NULL by default
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

	std::unique_ptr<std::thread> threadPlayerR(new std::thread(&AnimBirthPlayer, argPtr));
	threadPlayerR->detach();
	rPlayer_BS->Spawn() = true;
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
		std::unique_ptr<std::thread> thread_snd(new std::thread(&Explosion_tSnd, &sound));
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

void GameField::updPlayers()
{
	bool *keyArray(nullptr);

	auto fun = [=](Player& player, bool keyArray[])
	{
		if (
			(cr.cr_aPlayer == player.optTank.coef_reload && player.optTank.mod == playerModA && (keyArray[0] || keyArray[1] || keyArray[2] || keyArray[3])) ||
			(cr.cr_bPlayer == player.optTank.coef_reload && player.optTank.mod == playerModB && (keyArray[0] || keyArray[1] || keyArray[2] || keyArray[3])) ||
			(cr.cr_cPlayer == player.optTank.coef_reload && player.optTank.mod == playerModC && (keyArray[0] || keyArray[1] || keyArray[2] || keyArray[3])) ||
			(cr.cr_dPlayer == player.optTank.coef_reload && player.optTank.mod == playerModD && (keyArray[0] || keyArray[1] || keyArray[2] || keyArray[3])) ||
			(cr.cr_aEnemy == player.optTank.coef_reload && player.optTank.mod == enemyModA && (keyArray[0] || keyArray[1] || keyArray[2] || keyArray[3])) ||
			(cr.cr_bEnemy == player.optTank.coef_reload && player.optTank.mod == enemyModB && (keyArray[0] || keyArray[1] || keyArray[2] || keyArray[3])) ||
			(cr.cr_cEnemy == player.optTank.coef_reload && player.optTank.mod == enemyModC && (keyArray[0] || keyArray[1] || keyArray[2] || keyArray[3])) ||
			(cr.cr_dEnemy == player.optTank.coef_reload && player.optTank.mod == enemyModD && (keyArray[0] || keyArray[1] || keyArray[2] || keyArray[3])))
			player.reloadTank();

		//frames position
		player.setPosFrame(player.takeObj().getPosition().x, player.takeObj().getPosition().y);

		delete[] keyArray;
		keyArray = nullptr;
	};

	if (firstPlayer->Presence()) {
		keyArray = new bool[4]{ Key_A, Key_D, Key_W, Key_S };
		fun(*firstPlayer, keyArray);
	}

	if (secondPlayer->Presence()) {
		keyArray = new bool[4]{ Key_Left, Key_Right, Key_Up, Key_Down };
		fun(*secondPlayer, keyArray);
	}

	return;
}
