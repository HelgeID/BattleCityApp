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

	window.clear(sf::Color(127, 127, 127));
	DrawField();
	DrawMap();
	//DrawBlocks();
	//--------------------------------------
	DrawBrickDamage();
	//--------------------------------------

	DrawActors();
	std::for_each(tank.begin(), tank.end(), [&](Tank &tank) { DrawTank(tank); }); //DrawTank
	DrawBullets();

	window.draw(outsideUP);
	window.draw(outsideDOWN);
	window.draw(outsideLEFT);
	window.draw(outsideRIGHT);

	objTankCollision.MonitoringCollision(*this);
	objBulletCollision.MonitoringCollision(*this);
	objShootingBullets.MonitoringShootingBullets(*this);

	MonitoringKeys();
	CHECK_ACTOR_ON_COLLISION_ACTOR(firstPlayer, secondPlayer);
	CHECK_ACTOR_ON_COLLISION_FRAME(firstPlayer, secondPlayer);
	CHECK_ACTOR_ON_COLLISION_ENEMIES(firstPlayer, secondPlayer);
	CHECK_ACTOR_ON_COLLISION_BULLET(firstPlayer, secondPlayer);
	CHECK_ACTOR_ON_COLLISION_BLOCKS(firstPlayer, secondPlayer);

	clock.restart();

	//--------------------------------------
	//test bullet
	 //Bullet bulletObj(texture, sf::Vector2f(10.0, 10.0), UP);
	 //bulletObj.setPosFrame(10.0, 10.0);

	 //window.draw(bulletObj.takeObj());
	 //window.draw(bulletObj.frame);
	//test
	//--------------------------------------

	window.display();
	return;
}
