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
					{
						std::lock_guard<std::mutex> lg(mtx);
						gField->CreateAnimBirth(pos, index);
					}
					break;
				}
		}
		init_tank(pos);
	};

	auto random1 = [&](const size_t index)
	{
		sf::Vector2f pos_center{ 64.f, 26.f };
		random(index, pos_center);
	};

	auto random2 = [&](const size_t index)
	{
		sf::Vector2f pos_left{ 128.f, 80.f };
		sf::Vector2f pos_right{ 64.f, 100.f };
		random(index, pos_left);
		random(index, pos_right);
	};

	auto random3 = [&](const size_t index)
	{
		random1(index);
		random2(index);
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
	case 1: random1(0); break;
	case 2: random2(0); break;
	case 3: random3(0); break;
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

	switch (variant)
	{
	case 1: random3(1); break; // 1 + 3 == sum 4
	case 2: random2(2); break; // 2 + 2 == sum 4
	case 3: random1(3); break; // 3 + 1 == sum 4
	}

	mtx.lock();
	auto CollisionTanks = [&]()
	{
		bool flag(false);
		for (auto it1 = gField->tank.begin(); it1 != gField->tank.end(); ++it1) {
			for (auto it2(it1); it2 != gField->tank.end(); ++it2) {
				if (&*it1 == &*it2)
					continue;
				flag = it1->takeObj().getGlobalBounds().intersects(it2->takeObj().getGlobalBounds());
				if (flag)
					return flag;
			}
		}
		return flag;
	};
	mtx.unlock();

	int count(4);
	while (count)
		if (gField->tankAnimArr[count - 1].tankBirth == nullptr && !CollisionTanks())
			count--;
	{
		std::lock_guard<std::mutex> lg(mtx);
		std::for_each(gField->tank.begin(), gField->tank.end(), [&](Tank &tank) { tank.onTank(); });
	}
	return;
}
