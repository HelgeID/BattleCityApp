#include "field.h"
#include "general.hpp"
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

	UpdateCoefReload();

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

	window.display();

	return;
}

void GameField::UpdateTime()
{
	time = (float)clock.getElapsedTime().asMicroseconds();
	time_firstPlayer = clock_firstPlayer.getElapsedTime();
	time_secondPlayer = clock_secondPlayer.getElapsedTime();

	time_firstPlayer.asSeconds() > PlayerRechargeTime ? time_firstPlayer = sf::seconds(PlayerRechargeTime) : time_firstPlayer.Zero;
	time_secondPlayer.asSeconds() > PlayerRechargeTime ? time_secondPlayer = sf::seconds(PlayerRechargeTime) : time_secondPlayer.Zero;
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
