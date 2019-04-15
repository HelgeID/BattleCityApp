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
	case 1: bonus = new BonusTank(texture); break;
	case 2: bonus = new BonusSkin(texture); break;
	case 3: bonus = new BonusStar(texture); break;
	case 4: bonus = new BonusShovel(texture); break;
	case 5: bonus = new BonusClock(texture); break;
	case 6: bonus = new BonusGrenade(texture); break;
	}
}

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
	sf::sleep(sf::milliseconds(5000));
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
	permit_generation_tanks = false;

	if (completion_generation_tanks)
		return;

	int control(0);
	control = (p_player == 1 && (number_all_tanks - number_dead_tanks >= 4)) ? 4 : number_all_tanks - number_dead_tanks;
	control = (p_player == 2 && (number_all_tanks - number_dead_tanks >= 6)) ? 6 : number_all_tanks - number_dead_tanks;
	if (control) {
		std::cerr << "ok" << std::endl;
		std::unique_ptr<std::thread> thread_control(new std::thread(&LAUNCHING_TANKS_NUM, this, control));
		thread_control->detach();
	}
	std::cerr << control << std::endl;
	return;
}

void GameField::onBonusPistolFun(Player& player)
{
}
