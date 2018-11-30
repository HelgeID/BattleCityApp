#include "field.h"
#include <algorithm>

GameField::GameField(sf::RenderWindow &window, sf::Texture &texture)
	: window(window), texture(texture)
{
	CreateTanks();
}

GameField::~GameField()
{
}

void GameField::CreateTanks()
{
	Tank tankObj(texture);
	//tank.push_back(tankObj);
	//tank.push_back(tankObj);
	//tank.push_back(tankObj);
	//tank[0].loadTank(YELLOW, modA, DOWN); tank[0].setPosObj(32.f, 0.f);
	//tank[1].loadTank(GREEN, modC, DOWN); tank[1].setPosObj(64.f, 0.f);
	//tank[2].loadTank(RED, modF, DOWN); tank[2].setPosObj(128.f, 0.f);

	//tank.push_back(tankObj);
	//tank.push_back(tankObj);
	//tank[0].loadTank(YELLOW, modA, LEFT); tank[0].setPosObj(0.f, 96.f);
	//tank[1].loadTank(RED, modF, RIGHT); tank[1].setPosObj(184.f, 96.f);
	//tank[0].loadTank(RED, modF, RIGHT); tank[0].setPosObj(0.f, 96.f);
	//tank[1].loadTank(YELLOW, modA, LEFT); tank[1].setPosObj(184.f, 96.f);

	tank.push_back(tankObj);
	tank.push_back(tankObj);
	tank.push_back(tankObj);
	tank.push_back(tankObj);
	tank.push_back(tankObj);
	tank.push_back(tankObj);
	tank.push_back(tankObj);
	tank.push_back(tankObj);
	tank.push_back(tankObj);
	tank.push_back(tankObj);
	tank.push_back(tankObj);
	tank.push_back(tankObj);
	tank[0].loadTank(RED, modF, RIGHT); tank[0].setPosObj(0.f, 96.f);
	tank[1].loadTank(YELLOW, modA, LEFT); tank[1].setPosObj(184.f, 96.f);
	tank[2].loadTank(GREEN, modF, RIGHT); tank[2].setPosObj(0.f, 64.f);
	tank[3].loadTank(RED, modF, LEFT); tank[3].setPosObj(184.f, 64.f);
	tank[4].loadTank(WHITE, modB, RIGHT); tank[4].setPosObj(0.f, 184.f);
	tank[5].loadTank(GREEN, modC, LEFT); tank[5].setPosObj(184.f, 184.f);
	tank[6].loadTank(RED, modF, DOWN); tank[6].setPosObj(96.f, 0.f);
	tank[7].loadTank(YELLOW, modA, UP); tank[7].setPosObj(96.f, 184.f);
	tank[8].loadTank(GREEN, modF, DOWN); tank[8].setPosObj(64.f, 0.f);
	tank[9].loadTank(RED, modF, UP); tank[9].setPosObj(64.f, 184.f);
	tank[10].loadTank(WHITE, modB, DOWN); tank[10].setPosObj(184.f, 0.f);
	tank[11].loadTank(GREEN, modC, UP); tank[11].setPosObj(64.f, 184.f);
	return;
}

void GameField::DrawTank(Tank &tank)
{
	float speed = tank.optTank.speed;
	float time = this->time / speed;

	if (tank.optTank.dir == UP)
		tank.moveObj(0.f, -0.1f*time);
	else if (tank.optTank.dir == LEFT)
		tank.moveObj(-0.1f*time, 0.f);
	else if (tank.optTank.dir == DOWN)
		tank.moveObj(0.f, 0.1f*time);
	else if (tank.optTank.dir == RIGHT)
		tank.moveObj(0.1f*time, 0.f);

	if (coef_reload == tank.optTank.coef_reload)
		tank.reloadTank();

	window.draw(tank.takeObj());
	return;
}

void GameField::MoveTank(Tank& tank, float position)
{
	if (tank.optTank.dir == UP)
		tank.moveObj(0.f, -position);
	else if (tank.optTank.dir == LEFT)
		tank.moveObj(-position, 0.f);
	else if (tank.optTank.dir == DOWN)
		tank.moveObj(0.f, position);
	else if (tank.optTank.dir == RIGHT)
		tank.moveObj(position, 0.f);
	return;
}

void GameField::Monitoring()
{
	//coef_reload
	coef_reload++;
	if (coef_reload > 6)//todo max_coef_reload
		coef_reload = 0;

	for (auto it1 = tank.begin(); it1 != tank.end(); ++it1) {
		Collision(*it1);
		for (auto it2(it1); it2 != tank.end(); ++it2) {
			Collision(*it1, *it2);
		}
	}
	return;
}

void GameField::UpdateField()
{
	window.clear();
	time = (float)clock.getElapsedTime().asMicroseconds();

	std::for_each(tank.begin(), tank.end(), [&](Tank &tank) { DrawTank(tank); });

	Monitoring();
	clock.restart();
	window.display();
	return;
}
