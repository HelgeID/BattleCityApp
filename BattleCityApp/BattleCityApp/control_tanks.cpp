#include "field.h"
#include "general.hpp"
#include <time.h>

void CONTROL_TANKS(GameField* gField)
{
	auto init_tank = [&](sf::Vector2f pos) { std::lock_guard<std::mutex> lg(mtx); gField->CreateTank(pos); };

	auto random = [&](const size_t _index, const sf::Vector2f& pos)
	{
		{ //searching for an empty place
			for (size_t index = _index; index < 4; index++)
				if (gField->tankAnimArr[index].tankBirth == nullptr) {
					{ std::lock_guard<std::mutex> lg(mtx); gField->CreateAnimBirth(pos, index); }
					break;
				}
		}
		init_tank(pos);
	};

	auto random1 = [&](const size_t index, const bool flag)
	{
		mtx.lock();
			gField->c_BS->Spawn() = flag;
		mtx.unlock();

		//read position for animation
		sf::Vector2f pos_center{ gField->c_BS->getPosObj() };
		random(index, pos_center);
		return;
	};

	auto random2 = [&](const size_t index, const bool flag)
	{
		mtx.lock();
			gField->l_BS->Spawn() = flag;
			gField->r_BS->Spawn() = flag;
		mtx.unlock();

		//read position for animation
		sf::Vector2f pos_left{ gField->l_BS->getPosObj() };
		sf::Vector2f pos_right{ gField->r_BS->getPosObj() };
		random(index, pos_left);
		random(index, pos_right);
		return;
	};

	auto random3 = [&](const size_t index, const bool flag)
	{
		random1(index, flag);
		random2(index, flag);
		return;
	};

	//********************************************************
	//                      ** start **
	//********************************************************
	srand(time(NULL));
	int variant = (rand() % 3 + 1); variant = 1;//todo del
	sf::sleep(sf::milliseconds(2000));
	int index = variant - 1;
	///random ->> 1     *   
	///random ->> 2  *     *
	///random ->> 3  *  *  *

	switch (variant)
	{
	case 1: random1(0, false); break;
	case 2: random2(0, false); break;
	case 3: random3(0, false); break;
	}

	mtx.lock();
	auto check = [&](const int index)
	{
		if (!gField->tankAnimArr[index].tankBirth->FinishTime()) {
			return false;
		}
		else {
			gField->tank[index].onTank();
			return true;
		}
	};
	mtx.unlock();

	if (variant == 1) {
		while (!check(0))
			;
	}
	else
		if (variant == 2) {
			while (!check(0) || !check(1))
				;
		}
		else
			if (variant == 3) {
				while (!check(0) || !check(1) || !check(2))
					;
			}

	sf::sleep(sf::milliseconds(2000));
	blocking_firing = !blocking_firing;

	mtx.lock();
	auto CollisionTanks = [&]()
	{
		bool flag(false);
		for (auto it = gField->tank.begin(); it != gField->tank.end(); ++it) {
			if (it->takeObj().getGlobalBounds().intersects(gField->l_BS->takeObj().getGlobalBounds()))
				return true;
			if (it->takeObj().getGlobalBounds().intersects(gField->r_BS->takeObj().getGlobalBounds()))
				return true;
			if (it->takeObj().getGlobalBounds().intersects(gField->c_BS->takeObj().getGlobalBounds()))
				return true;
		}
		return false;
	};
	mtx.unlock();

	while (true) {
		if (CollisionTanks())
			;
		else
			break;
	}

	switch (variant)
	{
	case 1: random3(1, true); break; // 1 + 3 == sum 4
	case 2: random2(2, true); break; // 2 + 2 == sum 4
	case 3: random1(3, true); break; // 3 + 1 == sum 4
	}

	while (true) {
		if (gField->tankAnimArr[0].tankBirth == nullptr && gField->tankAnimArr[1].tankBirth == nullptr && gField->tankAnimArr[2].tankBirth == nullptr && gField->tankAnimArr[3].tankBirth == nullptr)
		{
			std::lock_guard<std::mutex> lg(mtx);
			gField->l_BS->Spawn() = false;
			gField->r_BS->Spawn() = false;
			gField->c_BS->Spawn() = false;
			break;
		}
	}

	{
		std::lock_guard<std::mutex> lg(mtx);
		std::for_each(gField->tank.begin(), gField->tank.end(), [&](Tank &tank) { tank.onTank(); });
	}
	return;
}
