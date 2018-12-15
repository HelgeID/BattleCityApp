#include "field.h"
#include "general.hpp"

void GameField::CreateTanks()
{
	Tank tankObj(texture);
	tank.push_back(tankObj);
	tank.push_back(tankObj);

	sf::Vector2f pos;
	tank[0].loadTank(YELLOW, modA, DOWN); pos = { 64.f, 16.f };//64, 16
	tank[0].setPosObj(pos.x, pos.y);

	tank[1].loadTank(YELLOW, modA, LEFT); pos = { 128.f, 80.f };//128, 80
	tank[1].setPosObj(pos.x, pos.y);

	//tank[0].mapPos.i = map.TakeIndex(pos, 'i');
	//tank[0].mapPos.j = map.TakeIndex(pos, 'j');
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
		CollisionFrame(*it1);
		CollisionBlocks(*it1);
		for (auto it2(it1); it2 != tank.end(); ++it2) {
			CollisionTanks(*it1, *it2);
		}
	}

	CollisionBullets();
	return;
}
