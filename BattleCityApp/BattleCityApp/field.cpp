#include "field.h"
#include "general.hpp"
#include <random>
#include <iostream>

GameField::GameField(sf::RenderWindow &window, sf::Texture &texture)
	: window(window), texture(texture)
{
	window.clear(sf::Color(127, 127, 127));
	FillField();
	FillMap();
	LoadMap();
	CreatePointsSpawnEnemy();
	InitOutside();
	CreateBlocks();
	CreateMoore();
	CreateActors();
	CreateTanks();

	std::unique_ptr<std::thread> thread_control(new std::thread(&ControlSound, this));
	thread_control->detach();
}

GameField::~GameField()
{
	delete firstPlayer;
	delete secondPlayer;
}

void GameField::UpdateField()
{
	UpdateTime();

	window.clear(sf::Color(127, 127, 127));
	DrawField();
	DrawMap();
	//DrawBlocks();
	DrawMoore();
	//--------------------------------------
	DrawBrickDamage();
	//--------------------------------------

	DrawActors();
	DrawTanks();
	DrawBullets();
	DrawAnimBirth();
	DrawAnimSkin();
	DrawAnimBoom();

	DrawBonus();

	UpdateCoefReload();
	UpdateDirectionTanks();

	//window.draw(outsideUP);
	//window.draw(outsideDOWN);
	//window.draw(outsideLEFT);
	//window.draw(outsideRIGHT);

	objTankCollision.MonitoringCollision(*this);
	objBulletCollision.MonitoringCollision(*this);
	objPlayerCollision.MonitoringCollision(*this);
	CheckOnMoore();
	CheckOnBonus();

	MonitoringKeys();
	objChecking.CheckingShootingBullets(*this);
	objChecking.CheckingSkin(*this);

	MonitoringAnim		(animBirth);
	MonitoringAnim		(animSkin);
	MonitoringAnim		(animBoom);

	clock.restart();

	f_f = !f_f; //for optimality
	return;
}

void GameField::UpdateTime()
{
	time = (float)clock.getElapsedTime().asMicroseconds();
	time_firstPlayer = clock_firstPlayer.getElapsedTime();
	time_secondPlayer = clock_secondPlayer.getElapsedTime();

	time_firstPlayer.asSeconds() > PlayerRechargeTime ? time_firstPlayer = sf::seconds(PlayerRechargeTime) : time_firstPlayer.Zero;
	time_secondPlayer.asSeconds() > PlayerRechargeTime ? time_secondPlayer = sf::seconds(PlayerRechargeTime) : time_secondPlayer.Zero;
	
	//
	//to do time here...

	bonus != nullptr ? bonus->UpdateTimer() : NULL;
	//

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
		it->isTank() ? u_p_d_t_i_m_e_t_a_n_k(*it) : NULL;

	for (auto it = tank.begin(); it != tank.end(); ++it)
		if (it->isTank() && it->optTank.mod == Model::enemyModD)
			u_p_d_h_e_a_v_y_t_a_n_k(*it);

	return;
}

void GameField::UpdateCoefReload()
{
	cr += 1; //coef_reload
	cr.cr_aPlayer > CoefReloadPlayerModA ? cr.cr_aPlayer = 0 : 0;
	cr.cr_bPlayer > CoefReloadPlayerModB ? cr.cr_bPlayer = 0 : 0;
	cr.cr_cPlayer > CoefReloadPlayerModC ? cr.cr_cPlayer = 0 : 0;
	cr.cr_dPlayer > CoefReloadPlayerModD ? cr.cr_dPlayer = 0 : 0;
	cr.cr_aEnemy > CoefReloadEnemyModA ? cr.cr_aEnemy = 0 : 0;
	cr.cr_bEnemy > CoefReloadEnemyModB ? cr.cr_bEnemy = 0 : 0;
	cr.cr_cEnemy > CoefReloadEnemyModC ? cr.cr_cEnemy = 0 : 0;
	cr.cr_dEnemy > CoefReloadEnemyModD ? cr.cr_dEnemy = 0 : 0;

	return;
}

void GameField::UpdateDirectionTanks()
{
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<int> dist(15, 30);

	sf::Time timeT;

	auto r_DIR = [&](Tank& tank) //Reverse Direction
	{
		const int posX(round(tank.takeObj().getPosition().x)), posY(round(tank.takeObj().getPosition().y));
		const bool bulletActivFlag(tank.optTankShooting.bulletActivFlag);
		tank.SetBoomCoord(posX, posY);
		tank.loadTank(
			tank.optTank.col,
			tank.optTank.mod,
			tank.optTank.dir = tank.ReverseDirection(tank.optTank.dir),
			tank.optTank.bonus
		);
		tank.optTankShooting.bulletActivFlag = bulletActivFlag;
		tank.setPosObj((float)posX, (float)posY);
		//std::cerr << "tankDir: " << spaceTank::myDirNames[tank.optTank.dir] << std::endl;
		//std::cerr << " :" << posX << " :" << posY << std::endl;

		tank.ResetBoomParam();
	};

	auto UPD = [&](Tank& tank) {
		timeT = tank.optRDir.tankClock_for_dir.getElapsedTime();
		if (timeT.asSeconds() > (float)tank.optRDir.tankTime_for_dir) {
			tank.optRDir.tankClock_for_dir.restart();
			tank.optRDir.tankTime_for_dir = dist(gen);
			r_DIR(tank);
		}
	};

	if (tank.size() == 0)
		return;

	for (auto it = tank.begin(); it != tank.end(); ++it)
		it->isTank() && !it->sleepTank() && it->optTank.step_speed ? UPD(*it) : NULL;

	return;
}
