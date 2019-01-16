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
	InitOutside();
	CreateBlocks();
	CreateActors();
	CreateTanks();

	//--------------------------------------
	//block[14].brickDamage(partsBrickVec, 0);
	//pbmap.fillMap(partsBrickVec[partsBrickVec.size() - 1]);

	//block[16].brickDamage(partsBrickVec, 0);
	//pbmap.fillMap(partsBrickVec[partsBrickVec.size() - 1]);
	//--------------------------------------

	//CreateBullet
	//CreateBullet(tank[0], sf::Vector2f(0.f, 0.f));
	//CreateBullet(tank[1], sf::Vector2f(0.f, 0.f));
}

GameField::~GameField()
{
	delete firstPlayer;
	delete secondPlayer;
}

void GameField::UpdateField()
{
	time = (float)clock.getElapsedTime().asMicroseconds();
	time_firstPlayer = clock_firstPlayer.getElapsedTime();
	time_secondPlayer = clock_secondPlayer.getElapsedTime();

	window.clear(sf::Color(127, 127, 127));
	DrawField();
	DrawMap();
	//DrawBlocks();
	//--------------------------------------
	DrawBrickDamage();
	//--------------------------------------

	DrawActors();
	DrawTanks();
	DrawBullets();
	DrawAnimBirth();
	DrawAnimSkin();
	DrawAnimBoom();


	//window.draw(outsideUP);
	//window.draw(outsideDOWN);
	//window.draw(outsideLEFT);
	//window.draw(outsideRIGHT);

	objTankCollision.MonitoringCollision(*this);
	objBulletCollision.MonitoringCollision(*this);
	objPlayerCollision.MonitoringCollision(*this);
	objShootingBullets.MonitoringShootingBullets(*this);

	MonitoringKeys();
	MonitoringAnim		(animBirth);
	MonitoringAnim		(animSkin);
	MonitoringAnim		(animBoom);

	clock.restart();

	window.display();
	return;
}
