#include "field.h"
#include "general.hpp"
#include <map>
#include <thread>

///////////////////////////////////////////////////////////////////////////////////////
#include <random>
auto random = [](const int a, const int b)
{
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<int> dist(a, b); //results between a and b inclusive
	return dist(gen); //random generation
};

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
	{ 90, enemyModB },{ 91, enemyModD },{ 92, enemyModD },{ 93, enemyModD },{ 94, enemyModC },{ 95, enemyModC },{ 96, enemyModD },{ 97, enemyModD },{ 98, enemyModD },{ 99, enemyModD }
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
	return;
}

void GameField::CreateTanks(const int numTanks)
{
	for (int i = 0; i < numTanks; i++) //max tank == numTanks, when one player or when two players
		CreateTank(sf::Vector2f(0.f, 0.f));

	number_all_tanks = mapOfEnemy.size();
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

	tank.optFM.random = random(1, 9);
	tank.optFM.clockTank.restart();

	//for a heavy tank
	if (tank.optTank.mod == enemyModD)
	{
		tank.loadTank(Color::GREEN, tank.optTank.mod, tank.optTank.dir, tank.optTank.bonus);
		tank.mapPos = { 0, 0 };
		tank.init_heavy_tank_damage();
	}

//	{
//		//Loaded tanks:
//		static int counter_loading(0);
//		std::cerr << "loading tank... " << ++counter_loading << std::endl;
//	}

	itMod++;
	if (itMod == mapOfEnemy.end())
	{
		//completion of generation of tanks
		itMod = mapOfEnemy.begin();
		completion_generation_tanks = true;
		std::cerr << "The END, completion of generation of tanks!!!" << std::endl;
	}
	else
		storage_tanks.EnableStorageTanks();

	return;
}

void GameField::MoveTank(Tank& tank, float position, bool inverse)
{
	switch (tank.optTank.dir)
	{
		case UP:		!inverse ? MoveTank(UP, tank, position) :		MoveTank(DOWN, tank, position);    break;
		case LEFT:		!inverse ? MoveTank(LEFT, tank, position) :		MoveTank(RIGHT, tank, position);   break;
		case DOWN:		!inverse ? MoveTank(DOWN, tank, position) :		MoveTank(UP, tank, position);      break;
		case RIGHT:		!inverse ? MoveTank(RIGHT, tank, position) :	MoveTank(LEFT, tank, position);    break;
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

bool GameField::DistanceTank(Tank& controlTank, const float distance)
{
	bool range(false);
	for (Tank &tank : tank)
	{
		if (tank.takeIndex() == controlTank.takeIndex())
			continue;

		tank.isTank() ? [&](bool& range) {
			const sf::Vector2f posTcentral(tank.getPosObj().x + 8, tank.getPosObj().y + 8);
			const sf::Vector2f posCTcentral(controlTank.getPosObj().x + 8, controlTank.getPosObj().y + 8);	
			range = (abs(posTcentral.x - posCTcentral.x) <= distance) && (abs(posTcentral.y - posCTcentral.y) <= distance);
		}(range) : NULL;
		if (range == true) {
			//std::cerr << " tank:" << tank.takeIndex() 
			//	<< " tank:" << controlTank.takeIndex()
			//<< std::endl;
			break;
		}
	}
	return range;
}

bool GameField::DistanceTank(Tank& tank1, Tank& tank2, const float distance)
{
	if (!tank1.isTank() && !tank2.isTank())
		return false;

	bool range(false);
	const sf::Vector2f posT1central(tank1.getPosObj().x + 8, tank1.getPosObj().y + 8);
	const sf::Vector2f posT2central(tank2.getPosObj().x + 8, tank2.getPosObj().y + 8);
	range = (abs(posT1central.x - posT2central.x) <= distance) && (abs(posT1central.y - posT2central.y) <= distance);
	if (range == true) {
		//std::cerr << " tank:" << tank1.takeIndex()
		//	<< " tank:" << tank2.takeIndex()
		//<< std::endl;
	}
	return range;
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

void GameField::CheckTankBang(const int indexTank, const int _indexTank, const bool killall)
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
					//play the bonus sound
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
			number_dead_tanks++;
			std::cerr << "Dead tanks:" << number_dead_tanks << std::endl;

			if (!killall) { //if did not take a bonus grenade
				//play the explosion sound
				std::unique_ptr<std::thread> thread_snd(new std::thread(&Explosion_fSnd, &sound));
				thread_snd->detach();
			}

			//add to UI statistics
			usesUI_nalltanks();

			if (_indexTank == 100) { //who owns the bullet
				number_killed_tanks_PL1 = number_killed_tanks_PL1 + 1;
				usesUI_ntanksforplayer1();
			}
			else if (_indexTank == 200) { //who owns the bullet
				number_killed_tanks_PL2 = number_killed_tanks_PL2 + 1;
				usesUI_ntanksforplayer2();
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

	permit_generation_tanks = false;

	std::vector<int> iT; //index tank
	std::for_each(tank.begin(), tank.end(), [&](Tank &tank) {
		if (tank.isTank())
			iT.push_back(tank.takeIndex());
	});

	for (int index = 0; index < iT.size(); index = index + 1)
		CheckTankBang(iT[index], 0, true); //kill all tanks

	bool allDead = true;
	std::for_each(tank.begin(), tank.end(), [&](Tank &tank) {
		if (tank.isTank()) //looking for alive
			allDead = false; //not all dead
	});

	if (!allDead)
		KillAllTanks(); //again repeat procedure

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
			if (!tank[index].isTank() || tank[index].sleepTank() || tank[index].optTank.mod == Model::enemyModD || tank[index].frontModeTank())
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

void GameField::ControlFrontMode()
{
	auto fmFun = [&](Tank &tank)
	{
		bool condition;

		//the tank does not rotate in the direction of the frame
		auto GetDIR = [&]()
		{
			if ((round(tank.takeObj().getPosition().x) == field.getPosition().x) && (tank.optTank.dir == UP || tank.optTank.dir == DOWN))
				return RIGHT;
			else if ((round(tank.takeObj().getPosition().x + 16) == (field.getPosition().x + field.getSize().x)) && (tank.optTank.dir == UP || tank.optTank.dir == DOWN))
				return LEFT;
			else if ((round(tank.takeObj().getPosition().y) == field.getPosition().y) && (tank.optTank.dir == LEFT || tank.optTank.dir == RIGHT))
				return DOWN;
			else if ((round(tank.takeObj().getPosition().y + 16) == (field.getPosition().y + field.getSize().y)) && (tank.optTank.dir == LEFT || tank.optTank.dir == RIGHT))
				return UP;
			else
				return tank.ClockWiseDirection(tank.optTank.dir);
		};

		//waiting for the necessary time (from 2.5sec to 22.5sec, 1-9 rnd)
		condition = (tank.optFM.clockTank.getElapsedTime().asSeconds() > tank.optFM.random * 2.5);

		//waiting for the desired position (x && y: 0, 8, 16, 32 ...)
		bool checkPos(
			((int)tank.getPosObj().x % 8 == 0 && (tank.optTank.dir == LEFT || tank.optTank.dir == RIGHT))
			||
			((int)tank.getPosObj().y % 8 == 0 && (tank.optTank.dir == UP || tank.optTank.dir == DOWN))
		);
		
		//if the distance to the tank < 24 && fm (frontMode) is off - do not turn on the mode
		if (DistanceTank(tank, 24.f) && tank.frontModeTank() == false)
			return;

		////////////////////////////////////////////////////////////
		// input into mode
		////////////////////////////////////////////////////////////
		if (condition && checkPos && tank.frontModeTank() == false)
		{
			tank.optFM.clockTank.restart();
			tank.optFM.random = random(1, 2);
			tank.frontModeTank() = true;

			//write new condition
			condition = tank.optFM.clockTank.getElapsedTime().asSeconds() > tank.optFM.random * 0.25;

			{
				const int posX(round(tank.takeObj().getPosition().x)), posY(round(tank.takeObj().getPosition().y));
				const bool bulletActivFlag(tank.optTankShooting.bulletActivFlag);

				tank.loadTank(
					tank.optTank.col,
					tank.optTank.mod,
					tank.optTank.dir = GetDIR(),
					tank.optTank.bonus
				);
				tank.mapPos = { 0, 0 };
				tank.optTankShooting.bulletActivFlag = bulletActivFlag;
				tank.setPosObj((float)posX, (float)posY);
			}
		}

		////////////////////////////////////////////////////////////
		// output from mode
		////////////////////////////////////////////////////////////
		if (condition && tank.frontModeTank() == true)
		{
			tank.optFM.clockTank.restart();
			tank.optFM.random = random(1, 9);
			tank.frontModeTank() = false;
		}
	};

////////////////////////////////////////////////////////////////////////
//START FUNC ControlFrontMode
////////////////////////////////////////////////////////////////////////

	if (tank.size() == 0)
		return;

	for (Tank &tank : tank)
		tank.isTank() && tank.takeIndex() % 2 == 0 && 
				tank.optTank.mod != enemyModB && tank.optTank.mod != enemyModD 
		
		? fmFun(tank) : NULL;

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
			MoveTank(tank, move, false);
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

		if (tank.sleepTank() || step_speed == 0.f || tank.frontModeTank())
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
