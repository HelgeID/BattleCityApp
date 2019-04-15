#include "field.h"
#include "general.hpp"
#include <map>
#include <thread>

std::map<int, Model> mapOfEnemy
{
	{ 0, enemyModA },{ 1, enemyModA },{ 2, enemyModA },{ 3, enemyModA },{ 4, enemyModA },{ 5, enemyModA },
	{ 6, enemyModA },{ 7, enemyModA },{ 8, enemyModA },{ 9, enemyModA },{ 10, enemyModA },{ 11, enemyModB },{ 12, enemyModD }
};

void GameField::CreateTanks()
{
	if (p_player == 1)
		for (int i = 0; i < 4; i++) //max tank == 4, when one player
			CreateTank(sf::Vector2f(0.f, 0.f));
	if (p_player == 2)
		for (int i = 0; i < 6; i++) //max tank == 6, when two players
			CreateTank(sf::Vector2f(0.f, 0.f));

	number_all_tanks = mapOfEnemy.size();

	std::unique_ptr<std::thread> thread_control(new std::thread(&LAUNCHING_TANKS, this));
	thread_control->detach();
	return;
}

void GameField::CreateTanks(const int numTanks)
{
	for (int i = 0; i < numTanks; i++) //max tank == numTanks, when one player or when two players
		CreateTank(sf::Vector2f(0.f, 0.f));

	std::unique_ptr<std::thread> thread_control(new std::thread(&LAUNCHING_TANKS_NUM, this, numTanks));
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

	//static int counter_loading(0);
	//
	//{
	//	//Loaded tanks:
	//	std::cerr << "loading tank... " << ++counter_loading << std::endl;
	//}

	itMod++;
	if (itMod == mapOfEnemy.end())
	{
		//completion of generation of tanks
		itMod = mapOfEnemy.begin();
		completion_generation_tanks = true;
		std::cerr << "END!!!" << std::endl;
	}

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
		//hit the bonus tank
		if (tank[index].takeIndex() == indexTank) {
			if (tank[index].optTank.bonus) {
				tank[index].optTank.bonus = !tank[index].optTank.bonus;
				tank[index].loadTank(Color::WHITE, 
					tank[index].optTank.mod,
					tank[index].optTank.dir,
					tank[index].optTank.bonus
				);
				tank[index].mapPos = { 0, 0 };
				if (!killall) { //if did not take a bonus grenade
					CreateBonus();
					std::unique_ptr<std::thread> thread_snd(new std::thread(&TakeBonusSnd, &sound));
					thread_snd->detach();
				}
				break;
			}

			const sf::Vector2f point = tank[index].getPosObj();
			CreateAnimBoom(point, "tankObj");
			tank[index].offTank();

			//init pos to NULL
			tank[index].setPosObj(0.f, 0.f);
			tank[index].setPosFrame(tank[index].getPosObj().x, tank[index].getPosObj().y);

			//number of dead tanks:
			{
				number_dead_tanks++;
				std::cerr << "Dead tanks:" << number_dead_tanks << std::endl;
			}

			if (!killall) { //if did not take a bonus grenade
				//play the explosion sound
				std::unique_ptr<std::thread> thread_snd(new std::thread(&Explosion_fSnd, &sound));
				thread_snd->detach();

				//launching a new tank on the field
				if (!completion_generation_tanks && number_dead_tanks <= number_all_tanks && !LAUNCHING_TANKS_ON_OFF) {
				//if (!completion_generation_tanks && number_dead_tanks <= number_all_tanks) {
					std::unique_ptr<std::thread> thread_control(new std::thread(&LOAD_TANK, this));
					thread_control->detach();
				}
			}

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
		CheckTankBang(iT[index], true); //kill all tanks

//	bool allDead = true;
//	std::for_each(tank.begin(), tank.end(), [&](Tank &tank) {
//		if (tank.isTank()) //looking for alive
//			allDead = false; //not all dead
//	});

//	if (!allDead)
//		KillAllTanks(); //again repeat procedure

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

void GameField::updTanks()
{
	auto fun = [=](Tank &tank)
	{
		const float step_speed = tank.optTank.step_speed;

		if (tank.sleepTank() || step_speed == 0.f)
			return;

		if (tank.optTank.dir == UP)
			tank.moveObj(0.f, -step_speed);
		else if (tank.optTank.dir == LEFT)
			tank.moveObj(-step_speed, 0.f);
		else if (tank.optTank.dir == DOWN)
			tank.moveObj(0.f, step_speed);
		else if (tank.optTank.dir == RIGHT)
			tank.moveObj(step_speed, 0.f);

		if (
			(cr.cr_aPlayer == tank.optTank.coef_reload && tank.optTank.mod == playerModA) ||
			(cr.cr_bPlayer == tank.optTank.coef_reload && tank.optTank.mod == playerModB) ||
			(cr.cr_cPlayer == tank.optTank.coef_reload && tank.optTank.mod == playerModC) ||
			(cr.cr_dPlayer == tank.optTank.coef_reload && tank.optTank.mod == playerModD) ||
			(cr.cr_aEnemy == tank.optTank.coef_reload && tank.optTank.mod == enemyModA) ||
			(cr.cr_bEnemy == tank.optTank.coef_reload && tank.optTank.mod == enemyModB) ||
			(cr.cr_cEnemy == tank.optTank.coef_reload && tank.optTank.mod == enemyModC) ||
			(cr.cr_dEnemy == tank.optTank.coef_reload && tank.optTank.mod == enemyModD))
			tank.reloadTank();

		//frames position
		tank.setPosFrame(tank.takeObj().getPosition().x, tank.takeObj().getPosition().y);
	};

	//--------------------------------------
	if (tank.size() == 0)
		return;

	std::for_each(tank.begin(), tank.end(), [&](Tank &tank) {
		tank.isTank() ? fun(tank) : NULL;
	});

	//--------------------------------------

	return;
}
