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

	//bonus
	bonus = new BonusTank(texture);
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

	MonitoringKeys();
	objChecking.CheckingShootingBullets(*this);
	objChecking.CheckingSkin(*this);

	MonitoringAnim		(animBirth);
	MonitoringAnim		(animSkin);
	MonitoringAnim		(animBoom);

	clock.restart();

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

	bonus->UpdateTimer();
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
			tank.loadTank_RED_WHITE();
			tank.optBonus.timeBonus = 0.f;
		}

		return;
	};

	for (auto it = tank.begin(); it != tank.end(); ++it)
		it->isTank() ? u_p_d_t_i_m_e_t_a_n_k(*it) : NULL;
	return;
}

void GameField::UpdateCoefReload()
{
	cr += 1; //coef_reload
	cr.cr_a > CoefReloadModA ? cr.cr_a = 0 : 0;
	cr.cr_b > CoefReloadModB ? cr.cr_b = 0 : 0;
	cr.cr_c > CoefReloadModC ? cr.cr_c = 0 : 0;
	cr.cr_d > CoefReloadModD ? cr.cr_d = 0 : 0;
	cr.cr_e > CoefReloadModE ? cr.cr_e = 0 : 0;
	cr.cr_f > CoefReloadModF ? cr.cr_f = 0 : 0;
	cr.cr_g > CoefReloadModG ? cr.cr_g = 0 : 0;
	cr.cr_h > CoefReloadModH ? cr.cr_h = 0 : 0;

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
		it->isTank() ? UPD(*it) : NULL;

	return;
}
