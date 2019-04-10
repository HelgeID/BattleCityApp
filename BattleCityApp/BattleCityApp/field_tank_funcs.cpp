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
	(tank.end() - 1)->mapPos = { 0, 0 };
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
	tank.mapPos = { 0, 0 };
	tank.setPosObj(pos.x, pos.y);

	//for a heavy tank
	if (tank.optTank.mod == enemyModD)
	{
		tank.loadTank(Color::GREEN, tank.optTank.mod, tank.optTank.dir, tank.optTank.bonus);
		tank.mapPos = { 0, 0 };
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
	tank.takeObj().getPosition().y < field.getPosition().y ? tank.moveObj(0.f, 1.f) : NULL;
	tank.takeObj().getPosition().y + 16 > field.getPosition().y + field.getSize().y ? tank.moveObj(0.f, -1.f) : NULL;
	tank.takeObj().getPosition().x < field.getPosition().x ? tank.moveObj(1.f, 0.f) : NULL;
	tank.takeObj().getPosition().x + 16 > field.getPosition().x + field.getSize().x ? tank.moveObj(-1.f, 0.f) : NULL;
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
				tank[index].mapPos = { 0, 0 };
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

void GameField::ControlHangPoint()
{
	sf::RectangleShape rectMap, rectTank;
	rectMap.setSize(sf::Vector2f(16, 16));
	rectTank.setSize(sf::Vector2f(16, 16));

	static int counter(0);
	if (++counter > 60)
	{
		for (int index(0); index < tank.size(); ++index)
		{
			if (!tank[index].isTank() || tank[index].sleepTank() || tank[index].optTank.mod == Model::enemyModD)
				continue;

			rectMap.setPosition(tank[index].mapPos.x, tank[index].mapPos.y);
			rectTank.setPosition((int)tank[index].getPosObj().x, (int)tank[index].getPosObj().y);

			if (rectMap.getGlobalBounds().intersects(rectTank.getGlobalBounds())) {
				//v1 (bad idea)
				//CheckTankBang(tank[index].takeIndex(), false);

				//v2
				const int posX(round(tank[index].takeObj().getPosition().x)), posY(round(tank[index].takeObj().getPosition().y));
				const bool bulletActivFlag(tank[index].optTankShooting.bulletActivFlag);

				tank[index].loadTank(
					tank[index].optTank.col,
					tank[index].optTank.mod,
					tank[index].optTank.dir = tank[index].ClockWiseDirection(tank[index].optTank.dir),
					tank[index].optTank.bonus
				);
				tank[index].mapPos = { 0, 0 };
				tank[index].optTankShooting.bulletActivFlag = bulletActivFlag;
				tank[index].setPosObj((float)posX, (float)posY);
			}

			else
			{
				tank[index].mapPos.x = (int)tank[index].getPosObj().x;
				tank[index].mapPos.y = (int)tank[index].getPosObj().y;
			}
		}
		counter = 0;
	}
	return;
}

void GameField::ControlBonusTank()
{
	//Update Time for tanks
	if (tank.size() == 0)
		return;

	auto u_p_d_t_i_m_e_t_a_n_k = [&](Tank& tank)
	{
		if (!tank.optTank.bonus)
			return;

		sf::Time deltaTime = tank.optBonus.clockTank.restart();
		tank.optBonus.timeBonus += deltaTime.asSeconds();
		if (tank.optBonus.timeBonus > 0.40f)
		{
			tank.switchColorTank(Color::RED, Color::WHITE);
			tank.optBonus.timeBonus = 0.f;
		}

		return;
	};

	for (auto it = tank.begin(); it != tank.end(); ++it)
		it->isTank() ? u_p_d_t_i_m_e_t_a_n_k(*it) : NULL;

	return;
}

void GameField::ControlHeavyTank()
{
	auto u_p_d_h_e_a_v_y_t_a_n_k = [&](Tank& tank)
	{
		static unsigned int t(0);

		tank.is_heavy_tank_damage_0() ? t > 10 ? tank.switchColorHeavyTank__WHITE(), t = 0 : 0 :
			tank.is_heavy_tank_damage_1() ? t > 10 ? tank.switchColorHeavyTank__YELLOW_WHITE(), t = 0 : 0 :
			tank.is_heavy_tank_damage_2() ? t > 10 ? tank.switchColorHeavyTank__YELLOW(), t = 0 : 0 :
			tank.is_heavy_tank_damage_3() ? t > 10 ? tank.switchColorHeavyTank__GREEN_YELLOW(), t = 0 : 0 :
			tank.is_heavy_tank_damage_4() ? t > 10 ? tank.switchColorHeavyTank__GREEN(), t = 0 : 0 :
			0;


		t += 1;
		return;
	};

	for (auto it = tank.begin(); it != tank.end(); ++it)
		if (it->isTank() && it->optTank.mod == Model::enemyModD)
			u_p_d_h_e_a_v_y_t_a_n_k(*it);

	return;
}

void GameField::RotationTank(Tank& tank, const char* choice_name_collision, const char* choice_name_rotation, const float move)
{
	/*
	choice_name_collision param:
	"collision_f" // frame (tank <=> frame)
	"collision_bs"  // blocks_spawn (tank <=> blocks_spawn)
	"collision_b" // blocks (tank <=> blocks)
	"collision_t" // tanks (tank <=> tank)
	"collision_p" // player (tank <=> player)
	"collision_m" // moore (tank <=> moore)
	"no_collision"
	*/

	/*
	choice_name_rotation param:
	"rotation_cw" // clock_wise
	"rotation_r"  // reverse
	"rotation_rr" // random_reverse
	*/

	Direction dirlast(tank.optTank.dir), dir; //get Tank Direction
	choice_name_rotation == "rotation_cw" ? dir = tank.ClockWiseDirection(dirlast) : NULL;
	choice_name_rotation == "rotation_r" ? dir = tank.ReverseDirection(dirlast) : NULL;
	choice_name_rotation == "rotation_rr" ? dir = tank.RandomReverseDirection(dirlast) : NULL;

	const int posX((int)round(tank.takeObj().getPosition().x)), posY((int)round(tank.takeObj().getPosition().y));
	const bool bulletActivFlag(tank.optTankShooting.bulletActivFlag);
	tank.SetBoomCoord(posX, posY);
	tank.loadTank(
		tank.optTank.col,
		tank.optTank.mod,
		tank.optTank.dir = dir,
		tank.optTank.bonus
	);
	tank.mapPos = { 0, 0 };
	tank.optTankShooting.bulletActivFlag = bulletActivFlag;
	tank.setPosObj((float)posX, (float)posY);

	if (choice_name_collision == "collision_f" || choice_name_collision == "collision_p" || choice_name_collision == "no_collision")
		; //move == 0.f
	else
		if (choice_name_collision == "collision_bs" || choice_name_collision == "collision_t") {
			MoveTank(tank, move);
		}
		else
			if (choice_name_collision == "collision_b" || choice_name_collision == "collision_m") {
				MoveTank(dirlast, tank, move);
			}

	tank.setPosFrame(tank.takeObj().getPosition().x, tank.takeObj().getPosition().y);

	//std::cerr << "tankDir: " << spaceTank::myDirNames[tank.optTank.dir] << std::endl;
	//std::cerr << " :" << posX << " :" << posY << std::endl;

	// call when there is a parameter "collision_b", "collision_m" - is done in the main collision check
	if (!(choice_name_collision == "collision_b" || choice_name_collision == "collision_m"))
		tank.ResetBoomParam();

	return;
};
