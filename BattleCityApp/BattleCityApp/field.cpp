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
	CreateBlocks();
	CreateTanks();

	//--------------------------------------
	block[14].onBrickDamage(partsBrickVec, 0);
	pbmap.fillMap(partsBrickVec[partsBrickVec.size() - 1]);

	block[16].onBrickDamage(partsBrickVec, 0);
	pbmap.fillMap(partsBrickVec[partsBrickVec.size() - 1]);
	//--------------------------------------
}

GameField::~GameField()
{
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

	std::for_each(tank.begin(), tank.end(), [&](Tank &tank) { DrawTank(tank); }); //DrawTank

	Monitoring();
	clock.restart();
	window.display();
	return;
}
