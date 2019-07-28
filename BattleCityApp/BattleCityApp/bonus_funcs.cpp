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
	std::uniform_int_distribution<std::mt19937::result_type> dist(1, 6);//1-6

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

void WaitingShovel(GameField* obj, const int value) //func to thread
{
	if (!no_close)
		return;

	{
		std::vector<int> iT; //index tank
		Object maskMoore(obj->texture, sf::Vector2f(32.f, 24.f));
		maskMoore.setPosObj(120.f, 200.f);
		for (size_t iTank(0); iTank < obj->tank.size(); ++iTank) {
			if (obj->tank[iTank].isTank()) {
				bool crossing = (maskMoore.takeObj().getGlobalBounds().intersects(obj->tank[iTank].frame.getGlobalBounds()));
				if (crossing)
					iT.push_back(obj->tank[iTank].takeIndex());
			}
		}

		for (int index = 0; index < iT.size(); index = index + 1)
			obj->CheckTankBang(iT[index], 0, true); //kill all tanks

		if (obj->firstPlayer->Presence()) {
			bool crossing = (maskMoore.takeObj().getGlobalBounds().intersects(obj->firstPlayer->frame.getGlobalBounds()));
			crossing ? obj->CheckPlayerBang(*obj->firstPlayer, true) : 0; //kill first player
		}

		if (obj->secondPlayer->Presence()) {
			bool crossing = (maskMoore.takeObj().getGlobalBounds().intersects(obj->secondPlayer->frame.getGlobalBounds()));
			crossing ? obj->CheckPlayerBang(*obj->secondPlayer, true) : 0; //kill first player
		}

	}

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

void WaitingClock(GameField* obj, const int value) //func to thread
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
	usesUI_nlifes();
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
	//call new thread for waiting shovel
	std::unique_ptr<std::thread> thread_waiting_shovel_start(new std::thread([&] {
		mThreads.callFuncInNewThread<GameField*>(&WaitingShovel, this);
	}));
	thread_waiting_shovel_start->detach();
	return;
}

void GameField::onBonusClockFun(Player& player)
{
	//call new thread for waiting clock
	std::unique_ptr<std::thread> thread_waiting_clock_start(new std::thread([&] {
		mThreads.callFuncInNewThread<GameField*>(&WaitingClock, this);
	}));
	thread_waiting_clock_start->detach();
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
