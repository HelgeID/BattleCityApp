﻿#include "field.h"
#include "general.hpp"
#include <random>
#include <iostream>

GameField::GameField(sf::RenderWindow &window, sf::Texture &texture)
	: window(window), texture(texture)
{
	{
		//wait
		window.clear(sf::Color(127, 127, 127));
		window.display();
		sf::sleep(sf::milliseconds(1500));
	}

	FillField();
	FillMap();
	LoadMap();
	CreatePointsSpawnEnemy();
	InitOutside();
	CreateBlocks();
	CreateMoore();
	CreateActors();
	CreateEmblem();
	CreateTanks();

	std::unique_ptr<std::thread> thread_ControlSpawnEnemies(new std::thread(&ControlSpawnEnemies, this));
	thread_ControlSpawnEnemies->detach();

	std::unique_ptr<std::thread> thread_ControlSound(new std::thread(&ControlSound, this));
	thread_ControlSound->detach();

	curtain = new Curtain(field);
	std::unique_ptr<std::thread> thread_ControlCurtain(new std::thread(&ControlCurtain, this));
	thread_ControlCurtain->detach();

	usesUI_nlifes();
	usesUI_nflags();
	usesUI_nalltanks();
	usesUI_ntanksforplayer1();
	usesUI_ntanksforplayer2();
}

GameField::~GameField()
{
	delete firstPlayer;
	delete secondPlayer;
}

void GameField::UpdateField()
{
	//--------------------------------------
	UpdateTime();
	//--------------------------------------
	window.clear(sf::Color(99, 99, 99));
	//--------------------------------------
	!pause ? updPlayers(), updTanks(), updBullets() : NULL;
	//--------------------------------------
	{
		//Draw game elements
		DrawField(); //ok
		DrawMap(); //ok
		DrawMoore(); //ok
		DrawBrickDamage(); //ok
		DrawEmblem(); //ok
		DrawBullets(); //ok
		DrawBonus(); //ok

		//--------------------------------------
		/*
		uses alternative: tMap for
		DrawBlocks, DrawActors, DrawTanks,
		DrawAnimBirth, DrawAnimSkin, DrawAnimBoom,
		DrawUI
		*/
		//--------------------------------------
		DrawDynamicElements(); //tDynamic
		DrawCurtain(); //ok

		window.draw(uiGameOverMSG.takeObj());
		window.draw(uiPauseMSG.takeObj());
	}

	//--------------------------------------
	READDATAOBJ(this); //get data for tDynamic
	//--------------------------------------
	!pause ? UpdateCoefReload(), UpdateDirectionTanks() : NULL;
	//--------------------------------------
	//window.draw(outsideUP); //for test
	//window.draw(outsideDOWN); //for test
	//window.draw(outsideLEFT); //for test
	//window.draw(outsideRIGHT); //for test
	//--------------------------------------
	!pause ?
		objTankCollision.MonitoringCollision(*this),
		objBulletCollision.MonitoringCollision(*this),
		objPlayerCollision.MonitoringCollision(*this),
		CheckOnMoore(), CheckOnEmblem(), CheckOnBonus()
		: NULL;
	//--------------------------------------
	MonitoringKeys();
	!pause ? objChecking.CheckingShootingBullets(*this), objChecking.CheckingSkin(*this) : NULL;
	//--------------------------------------
	!pause ?
		ControlHangPoint(),
		ControlBonusTank(),
		ControlHeavyTank(),
		ControlFrontMode()
		: NULL;
	//--------------------------------------
	MonitoringAnim		(animBirth);
	MonitoringAnim		(animSkin);
	MonitoringAnim		(animBoom);
	//--------------------------------------
	clock.restart();
	//--------------------------------------
	f_f = !f_f; //for optimality
	//--------------------------------------

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

	//Reverse Direction
	auto r_DIR = [&](Tank& tank) { RotationTank(tank, "no_collision", "rotation_r", 0.f); return; };

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
	{
		it->isTank() //presence of the tank on the map
			&& !it->sleepTank() //tank is not in sleep modes
			&& it->optTank.step_speed //step speed is not zero
			&& it->optTank.mod != Model::enemyModD //not a heavy tank
			&& !it->frontModeTank()
				? UPD(*it) : NULL;

		if (it->optTank.mod == Model::enemyModD)
			it->optRDir.tankClock_for_dir.restart();
	}

	return;
}


//GameOver
void GameField::GameOver()
{
	emblem.CrushEmblem();
	gameover = true;
	StartGameOverMSG();

	std::unique_ptr<std::thread> CLOSE_THREAD(new std::thread(
		[]() -> void {
		sf::sleep(sf::milliseconds(8000));
		if (no_close)
			no_close = false;
		return;
	}));
	CLOSE_THREAD->detach();
	return;
}
