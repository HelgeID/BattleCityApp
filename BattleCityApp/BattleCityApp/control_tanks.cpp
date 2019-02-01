#include "field.h"
#include "general.hpp"
#include <time.h>

static bool LAUNCHING_TANKS_ON_START_FINISH = false; //todo restart

bool CONTROL_CollisionTanksBS(GameField* gField)
{
	for (auto it = gField->tank.begin(); it != gField->tank.end(); ++it) {
			if (!it->isTank())
				continue;

		if (it->takeObj().getGlobalBounds().intersects(gField->l_BS->takeObj().getGlobalBounds()))
			return true;
		if (it->takeObj().getGlobalBounds().intersects(gField->r_BS->takeObj().getGlobalBounds()))
			return true;
		if (it->takeObj().getGlobalBounds().intersects(gField->c_BS->takeObj().getGlobalBounds()))
			return true;
	}
	return false;
}

bool CONTROL_CollisionTanksTank(GameField* gField, Tank& tank)
{
	for (auto it = gField->tank.begin(); it != gField->tank.end(); ++it) {
		if (&*it == &tank)
			continue;
		if (it->takeObj().getGlobalBounds().intersects(tank.takeObj().getGlobalBounds()))
			return true;
	}
	return false;
}

bool CONTROL_CheckFinishTimeAnim(GameField* gField, const int index)
{
	std::lock_guard<std::mutex> lg(mtx);

	if (gField->tankAnimArr[index].tankBirth == nullptr) {
		gField->tank[index].onTank();
		return true;
	}

	if (!gField->tankAnimArr[index].tankBirth->FinishTime()) {
		return false;
	}
	else {
		gField->tank[index].onTank();
		return true;
	}
}

bool CONTROL_OffAllAnim(GameField* gField)
{
	return (gField->tankAnimArr[0].tankBirth == nullptr &&
		gField->tankAnimArr[1].tankBirth == nullptr &&
		gField->tankAnimArr[2].tankBirth == nullptr &&
		gField->tankAnimArr[3].tankBirth == nullptr);
}

void LAUNCHING_TANKS(GameField* gField)
{
	srand(time(NULL));

	auto init_tank = [&](sf::Vector2f pos) { std::lock_guard<std::mutex> lg(mtx); gField->CreateTank(pos); };

	auto create_anim = [&](const size_t _index, const sf::Vector2f& pos)
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
		create_anim(index, pos_center);
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
		create_anim(index, pos_left);
		create_anim(index, pos_right);
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
	//search variant
	int variant = (rand() % 3 + 1);

	///random ->> 1     *   
	///random ->> 2  *     *
	///random ->> 3  *  *  *

	//part 1
	sf::sleep(sf::milliseconds(2000));

	switch (variant)
	{
		case 1: random1(0, false); break;
		case 2: random2(0, false); break;
		case 3: random3(0, false); break;
	}

	if (variant == 1) {
		while (!CONTROL_CheckFinishTimeAnim(gField, 0))
			;
	}
	else
		if (variant == 2) {
			while (!CONTROL_CheckFinishTimeAnim(gField, 0) || !CONTROL_CheckFinishTimeAnim(gField, 1))
				;
		}
		else
			if (variant == 3) {
				while (!CONTROL_CheckFinishTimeAnim(gField, 0) || !CONTROL_CheckFinishTimeAnim(gField, 1) || !CONTROL_CheckFinishTimeAnim(gField, 2))
					;
			}
	
	//part 2
	sf::sleep(sf::milliseconds(2000));
	
	while (CONTROL_CollisionTanksBS(gField))
		sf::sleep(sf::milliseconds(300));

	switch (variant)
	{
		case 1: random3(1, true); break; // 1 + 3 == sum 4
		case 2: random2(2, true); break; // 2 + 2 == sum 4
		case 3: random1(3, true); break; // 3 + 1 == sum 4
	}

	while (true) {
		if (CONTROL_OffAllAnim(gField))
		{
			std::lock_guard<std::mutex> lg(mtx);
			gField->l_BS->Spawn() = false;
			gField->r_BS->Spawn() = false;
			gField->c_BS->Spawn() = false;
			break;
		}
		sf::sleep(sf::milliseconds(300));
	}

	{
		std::lock_guard<std::mutex> lg(mtx);

		std::vector<Tank>::iterator first = gField->tank.begin();
		std::vector<Tank>::iterator last = gField->tank.begin() + variant;
		const int distance = std::distance(first, last);

		int indexTank(0);
		std::for_each(gField->tank.begin(), gField->tank.end(), [&](Tank &tank) 
		{
			if (indexTank >= distance)
				tank.onTank();
			indexTank = indexTank + 1;
		});

	}
	LAUNCHING_TANKS_ON_START_FINISH = true;
	std::cout << "\a";
	return;
}

void ON_TANK(GameField* gField)
{
	srand(time(NULL));

	auto reload_tank = [&](sf::Vector2f pos, int& indexTank) { 
		std::lock_guard<std::mutex> lg(mtx);
		int count(0);
		for (auto it = gField->tank.begin(); it != gField->tank.end(); ++it) {
			if (!it->isTank()) {
				gField->ReloadTank(*it, pos);
				indexTank = count;
				break;
			}
			++count;
		}
		return;
	};

	auto create_anim = [&](const sf::Vector2f& pos)
	{
		size_t _index;
		{ //searching for an empty place
		research:
			;
			for (size_t index = 0; index < 4; index++) {
				if (gField->tankAnimArr[index].tankBirth == nullptr) {
					{ std::lock_guard<std::mutex> lg(mtx); gField->CreateAnimBirth(pos, index); }
					_index = index;
					goto exit;
				}
			}

			sf::sleep(sf::milliseconds(300));
			goto research;
		}
	exit:
		;
		return _index;
	};

	//********************************************************
	//                      ** start **
	//********************************************************
	while (!LAUNCHING_TANKS_ON_START_FINISH)
		sf::sleep(sf::milliseconds(300));

	sf::sleep(sf::milliseconds(2000));

	sf::Vector2f pos{ 0.f, 0.f };
	int place; // "l" or "r" or "c"

	while (CONTROL_CollisionTanksBS(gField))
		sf::sleep(sf::milliseconds(300));

	while (true)
	{
		place = (rand() % 3 + 1);

		if (place == 1 && !gField->l_BS->Spawn()) {
			gField->l_BS->Spawn() = true;
			pos = gField->l_BS->getPosObj();
			break;
		}
			
		if (place == 2 && !gField->r_BS->Spawn()) {
			gField->r_BS->Spawn() = true;
			pos = gField->r_BS->getPosObj();
			break;
		}

		if (place == 3 && !gField->c_BS->Spawn()) {
			gField->c_BS->Spawn() = true;
			pos = gField->c_BS->getPosObj();
			break;
		}
	}

	//get the index of the created animation
	size_t ind = create_anim(pos);

	//waiting for the animation to finish
	while (gField->tankAnimArr[ind].tankBirth != nullptr)
		;

	switch (place)
	{
		case 1: gField->l_BS->Spawn() = false; break;
		case 2: gField->r_BS->Spawn() = false; break;
		case 3: gField->c_BS->Spawn() = false; break;
	}

	int indexTank(-1);
	reload_tank(pos, indexTank);

	if (indexTank != -1) {
		while (CONTROL_CollisionTanksTank(gField, gField->tank[indexTank]))
			sf::sleep(sf::milliseconds(300));
		gField->tank[indexTank].onTank();
	}

	return;
}
