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
	UpdateTime();

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

	MonitoringKeys();
	objChecking.CheckingShootingBullets(*this);
	objChecking.CheckingSkin(*this);

	MonitoringAnim		(animBirth);
	MonitoringAnim		(animSkin);
	MonitoringAnim		(animBoom);

	clock.restart();

	window.display();

	//test
	{
		std::cerr << std::endl; std::cerr << "Birth:";
		firstPlayerAnim.playerBirth == nullptr ? std::cerr << 0 : std::cerr << 1;
		secondPlayerAnim.playerBirth == nullptr ? std::cerr << 0 : std::cerr << 1;
		std::cerr << std::endl; std::cerr << "Skin:";
		firstPlayerAnim.playerSkin == nullptr ? std::cerr << 0 : std::cerr << 1;
		secondPlayerAnim.playerSkin == nullptr ? std::cerr << 0 : std::cerr << 1;
		std::cerr << std::endl;

		std::cerr << std::endl; std::cerr << "Boom:";
		auto test = [&](const size_t i) {
			if (bulletBoom[i] == nullptr)
				std::cerr << 0;
			else
				std::cerr << 1;
		};
		for (size_t i(0); i < 6; i++)
			test(i);

	}
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
