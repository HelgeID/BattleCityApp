#include <thread>
#include "field.h"
#include "general.hpp"

void WaitingShovel(GameField* obj)
{
	mtx.lock(); obj->CreateMoore("steel"); mtx.unlock();
	sf::sleep(sf::milliseconds(20000));

	int count(10);
	while (count)
	{
		mtx.lock(); obj->CreateMoore("steel"); mtx.unlock();
		sf::sleep(sf::milliseconds(200));
		mtx.lock(); obj->CreateMoore("brick"); mtx.unlock();
		sf::sleep(sf::milliseconds(200));
		--count;
	}
	return;
}

void WaitingClock(GameField* obj)
{
	auto sleep = [&](bool f) {
		obj->sleeptanks = f;
		std::for_each(obj->tank.begin(), obj->tank.end(), [&](Tank &tank) {
			tank.isTank() ? tank.sleepTank() = f : NULL;
		});
	};

	mtx.lock(); sleep(true); mtx.unlock(); //on sleep
	sf::sleep(sf::milliseconds(60000));
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
	PerfectionPlayer(player);
	return;
}

void GameField::onBonusShovelFun(Player& player)
{
	std::unique_ptr<std::thread> threadPlayer(new std::thread(&WaitingShovel, this));
	threadPlayer->detach();
}

void GameField::onBonusClockFun(Player& player)
{
	std::unique_ptr<std::thread> threadPlayer(new std::thread(&WaitingClock, this));
	threadPlayer->detach();
	return;
}

void GameField::onBonusGrenadeFun(Player& player)
{
}

void GameField::onBonusPistolFun(Player& player)
{
}
