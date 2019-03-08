#include "field.h"
#include "general.hpp"
#include <map>
#include <thread>

std::map<int, Model> mapOfEnemy
{
	{ 0, enemyModA },{ 1, enemyModA },{ 2, enemyModA },{ 3, enemyModA },{ 4, enemyModA },{ 5, enemyModA },{ 6, enemyModA },{ 7, enemyModA },{ 8, enemyModA },{ 9, enemyModA },
	{ 10, enemyModA },{ 11, enemyModB },{ 12, enemyModC },{ 13, enemyModB },{ 14, enemyModA },{ 15, enemyModA },{ 16, enemyModC },{ 17, enemyModA },{ 18, enemyModA },{ 19, enemyModC },
	{ 20, enemyModC },{ 21, enemyModC },{ 22, enemyModA },{ 23, enemyModC },{ 24, enemyModA },{ 25, enemyModB },{ 26, enemyModD },{ 27, enemyModB },{ 28, enemyModC },{ 29, enemyModA },
	{ 30, enemyModD },{ 31, enemyModC },{ 32, enemyModA },{ 33, enemyModA },{ 34, enemyModC },{ 35, enemyModA },{ 36, enemyModC },{ 37, enemyModD },{ 38, enemyModB },{ 39, enemyModC },
	{ 40, enemyModB },{ 41, enemyModD },{ 42, enemyModA },{ 43, enemyModB },{ 44, enemyModC },{ 45, enemyModD },{ 46, enemyModA },{ 47, enemyModA },{ 48, enemyModD },{ 49, enemyModA },
	{ 50, enemyModD },{ 51, enemyModB },{ 52, enemyModD },{ 53, enemyModC },{ 54, enemyModD },{ 55, enemyModA },{ 56, enemyModA },{ 57, enemyModD },{ 58, enemyModA },{ 59, enemyModA },
	{ 60, enemyModA },{ 61, enemyModD },{ 62, enemyModA },{ 63, enemyModB },{ 64, enemyModD },{ 65, enemyModD },{ 66, enemyModB },{ 67, enemyModB },{ 68, enemyModD },{ 69, enemyModD },
	{ 70, enemyModD },{ 71, enemyModD },{ 72, enemyModD },{ 73, enemyModB },{ 74, enemyModA },{ 75, enemyModD },{ 76, enemyModC },{ 77, enemyModA },{ 78, enemyModC },{ 79, enemyModD },
	{ 80, enemyModD },{ 81, enemyModC },{ 82, enemyModD },{ 83, enemyModC },{ 84, enemyModD },{ 85, enemyModC },{ 86, enemyModC },{ 87, enemyModD },{ 88, enemyModC },{ 89, enemyModA },
	{ 90, enemyModB },{ 91, enemyModD },{ 92, enemyModD },{ 93, enemyModD },{ 94, enemyModC },{ 95, enemyModC },{ 96, enemyModD },{ 7, enemyModD },{ 98, enemyModD },{ 99, enemyModD }
};

void GameField::CreateTanks()
{
	if (p_player == 1)
		for (int i = 0; i < 4; i++) //max tank == 4, when one player
			CreateTank(sf::Vector2f(0.f, 0.f));
	if (p_player == 2)
		for (int i = 0; i < 6; i++) //max tank == 6, when two players
			CreateTank(sf::Vector2f(0.f, 0.f));

	std::unique_ptr<std::thread> thread_control(new std::thread(&LAUNCHING_TANKS, this));
	thread_control->detach();
	return;
}

void GameField::CreateTank(const sf::Vector2f pos)
{
	/////////////////////////
	//default function Create
	/////////////////////////
	static Tank tankObj(texture);
	tank.push_back(tankObj);
	(tank.end() - 1)->loadTank(RED, enemyModA, DOWN, false);
	(tank.end() - 1)->loadIndex(tank);
	(tank.end() - 1)->setPosObj(pos.x, pos.y);
	return;
}

void GameField::ReloadTank(Tank& tank, const sf::Vector2f pos)
{
	static std::map<int, Model>::iterator itMod = mapOfEnemy.begin();//todo restart
	bool bonus(false);
	const unsigned int bonusPosArr[]{ 4, 11, 18, 25, 32, 39, 46, 53, 60, 67, 74, 81, 88, 95 };
	size_t sizebonusPosArr(sizeof(bonusPosArr) / sizeof(unsigned int));
	for (size_t i(0); i < sizebonusPosArr; ++i)
		if (bonusPosArr[i] == itMod->first)
			bonus = true;

	tank.loadTank(WHITE, itMod->second, DOWN, bonus);
	tank.setPosObj(pos.x, pos.y);

	//for a heavy tank
	if (tank.optTank.mod == enemyModD)
	{
		tank.loadTank(Color::GREEN, tank.optTank.mod, tank.optTank.dir, tank.optTank.bonus);
		tank.init_heavy_tank_damage();
	}

	if (itMod != mapOfEnemy.end())
		itMod++;

	return;
}

void GameField::MoveTank(Tank& tank, float position)
{
	switch (tank.optTank.dir)
	{
		case UP: MoveTank(UP, tank, position); break;
		case LEFT: MoveTank(LEFT, tank, position); break;
		case DOWN: MoveTank(DOWN, tank, position); break;
		case RIGHT: MoveTank(RIGHT, tank, position); break;
	}
	return;
}

void GameField::MoveTank(const Direction dir, Tank& tank, float position)
{
	if (dir == UP)
		tank.moveObj(0.f, -position);
	else if (dir == LEFT)
		tank.moveObj(-position, 0.f);
	else if (dir == DOWN)
		tank.moveObj(0.f, position);
	else if (dir == RIGHT)
		tank.moveObj(position, 0.f);
	return;
}

//todo fun
void GameField::ControlTank_onFrame(Tank& tank)
{
	tank.takeObj().getPosition().y < field.getPosition().y ? tank.moveObj(0.f, 2.f) : NULL;
	tank.takeObj().getPosition().y + 16 > field.getPosition().y + field.getSize().y ? tank.moveObj(0.f, -2.f) : NULL;
	tank.takeObj().getPosition().x < field.getPosition().x ? tank.moveObj(2.f, 0.f) : NULL;
	tank.takeObj().getPosition().x + 16 > field.getPosition().x + field.getSize().x ? tank.moveObj(-2.f, 0.f) : NULL;
	return;
}

void GameField::CheckTankBang(const int indexTank, const bool killall)
{
	if (undying_enemy)
		return;

	//off tank
	for (int index(0); index < tank.size(); ++index) {
		if (tank[index].takeIndex() == indexTank) {
			if (tank[index].optTank.bonus) {
				tank[index].optTank.bonus = !tank[index].optTank.bonus;
				tank[index].loadTank(Color::WHITE, 
					tank[index].optTank.mod,
					tank[index].optTank.dir,
					tank[index].optTank.bonus
				);

				CreateBonus();
				std::unique_ptr<std::thread> thread_snd(new std::thread(&TakeBonusSnd, &sound));
				thread_snd->detach();
				break;
			}

			const sf::Vector2f point = tank[index].getPosObj();
			CreateAnimBoom(point, "tankObj");
			tank[index].offTank();

			std::unique_ptr<std::thread> thread_snd(new std::thread(&Explosion_fSnd, &sound));
			thread_snd->detach();

			//init pos to NULL
			tank[index].setPosObj(0.f, 0.f);
			tank[index].setPosFrame(tank[index].getPosObj().x, tank[index].getPosObj().y);

			if (!killall) {
				//launching a new tank on the field
				std::unique_ptr<std::thread> thread_control(new std::thread(&LOAD_TANK, this, false));
				thread_control->detach();
			}
			else
				RemovalObj(tank, index);

			break;
		}
	}
	return;
}

void GameField::KillAllTanks()
{
	if (tank.size() == 0)
		return;

	std::vector<int> iT; //index tank
	std::for_each(tank.begin(), tank.end(), [&](Tank &tank) {
		if (tank.isTank())
			iT.push_back(tank.takeIndex());
	});

	for (int index = 0; index < iT.size(); index = index + 1)
		CheckTankBang(iT[index], true);

	bool allFlg = true;
	std::for_each(tank.begin(), tank.end(), [&](Tank &tank) {
		if (tank.isTank())
			allFlg = false;
	});

	if (!allFlg)
		KillAllTanks();

	if (tank.size() > 0)
		tank.clear(); //clear array of tanks, who were not killed
	return;
}
