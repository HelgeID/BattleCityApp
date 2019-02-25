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

void GameField::onBonusTankFun(Tank& player)
{
}

void GameField::onBonusSkinFun(Tank& player)
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

void GameField::onBonusStarFun(Tank& player)
{
}

void GameField::onBonusShovelFun(Tank& player)
{
	std::unique_ptr<std::thread> threadPlayer(new std::thread(&WaitingShovel, this));
	threadPlayer->detach();
}

void GameField::onBonusClockFun(Tank& player)
{
}

void GameField::onBonusGrenadeFun(Tank& player)
{
}

void GameField::onBonusPistolFun(Tank& player)
{
}
