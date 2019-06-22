#include <thread>
#include <random>
#include "field.h"
#include "general.hpp"

void GameField::CreateBonus()
{
	if (bonus != nullptr)
		delete bonus;

	std::random_device dev;
	std::mt19937 generator(dev());
	std::uniform_int_distribution<std::mt19937::result_type> dist(6, 6);//1-6

	switch (dist(generator))
	{
	case 1: bonus = new BonusTank(texture, firstPlayer->getPosObj(), secondPlayer->getPosObj()); break;
	case 2: bonus = new BonusSkin(texture, firstPlayer->getPosObj(), secondPlayer->getPosObj()); break;
	case 3: bonus = new BonusStar(texture, firstPlayer->getPosObj(), secondPlayer->getPosObj()); break;
	case 4: bonus = new BonusShovel(texture, firstPlayer->getPosObj(), secondPlayer->getPosObj()); break;
	case 5: bonus = new BonusClock(texture, firstPlayer->getPosObj(), secondPlayer->getPosObj()); break;
	case 6: bonus = new BonusGrenade(texture, firstPlayer->getPosObj(), secondPlayer->getPosObj()); break;
	}
}

void WaitingShovel(GameField* obj) //func to thread
{
	if (!no_close)
		return;

	mtx.lock(); obj->CreateMoore("steel"); mtx.unlock();
	sf::sleep(sf::milliseconds(20000));

	if (!no_close)
		return;

	int count(10);
	while (count && no_close)
	{
		mtx.lock(); obj->CreateMoore("steel"); mtx.unlock();
		sf::sleep(sf::milliseconds(200));
		mtx.lock(); obj->CreateMoore("brick"); mtx.unlock();
		sf::sleep(sf::milliseconds(200));
		--count;
	}
	return;
}

void WaitingClock(GameField* obj) //func to thread
{
	auto sleep = [&](bool f) {
		obj->sleeptanks = f;
		std::for_each(obj->tank.begin(), obj->tank.end(), [&](Tank &tank) {
			tank.isTank() ? tank.sleepTank() = f : NULL;
		});
	};

	if (!no_close)
		return;

	mtx.lock(); sleep(true); mtx.unlock(); //on sleep
	sf::sleep(sf::milliseconds(8000));

	if (!no_close)
		return;

	mtx.lock(); sleep(false); mtx.unlock(); //off sleep
	return;
}

void GameField::onBonusTankFun(Player& player)
{
	player.inclife();
	return;
}

void GameField::onBonusSkinFun(Player& player)
{
	if (!player.GetSkin())
		player.SkinOn();
	else {
		if (player.name == "first player")
			firstPlayerAnim.playerSkin->elapsedTime = 
			firstPlayerAnim.playerSkin->elapsedClock.restart();

		else if (player.name == "second player")
			secondPlayerAnim.playerSkin->elapsedTime = 
			secondPlayerAnim.playerSkin->elapsedClock.restart();
	}
	return;
}

void GameField::onBonusStarFun(Player& player)
{
	player.numStar() = player.numStar() + 1;
	player.numStar() > 3 ? player.numStar() = 3
		: PerfectionPlayer(player);
	return;
}

void GameField::onBonusShovelFun(Player& player)
{
	std::unique_ptr<std::thread> waitingshovel(new std::thread(&WaitingShovel, this));
	waitingshovel->detach();
}

void GameField::onBonusClockFun(Player& player)
{
	std::unique_ptr<std::thread> waitingclock(new std::thread(&WaitingClock, this));
	waitingclock->detach();
	return;
}

void GameField::onBonusGrenadeFun(Player& player)
{
	KillAllTanks(); // KILL ALL TANKS
	return;
}

void GameField::onBonusPistolFun(Player& player)
{
}
