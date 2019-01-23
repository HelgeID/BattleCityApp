#include "field.h"
#include "general.hpp"

void GameField::CreateTanks()
{
	//todo new thread
	Tank tankObj(texture);
	tank.push_back(tankObj);
	tank.push_back(tankObj);

	sf::Vector2f pos;
	tank[0].loadTank(YELLOW, modA, DOWN); pos = { 64.f, 16.f };//64, 16
	tank[0].setPosObj(pos.x, pos.y);
	tank[0].loadIndex(tank);
	tank[0].onTank();

	tank[1].loadTank(YELLOW, modA, LEFT); pos = { 128.f, 80.f };//128, 80
	tank[1].setPosObj(pos.x, pos.y);
	tank[1].loadIndex(tank);
	tank[1].onTank();

	//tank[0].mapPos.i = map.TakeIndex(pos, 'i');
	//tank[0].mapPos.j = map.TakeIndex(pos, 'j');

	//RemovalObj(tank, 0);
	//RemovalObj(tank, 1);

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

//todo fun
void GameField::ControlTank_onFrame(Tank& tank)
{
	tank.takeObj().getPosition().y < field.getPosition().y ? tank.moveObj(0.f, 2.f) : NULL;
	tank.takeObj().getPosition().y + 16 > field.getPosition().y + field.getSize().y ? tank.moveObj(0.f, -2.f) : NULL;
	tank.takeObj().getPosition().x < field.getPosition().x ? tank.moveObj(2.f, 0.f) : NULL;
	tank.takeObj().getPosition().x + 16 > field.getPosition().x + field.getSize().x ? tank.moveObj(-2.f, 0.f) : NULL;
	return;
}

void GameField::CheckTankBang(const int indexTank)
{
	if (undying_enemy)
		return;

	//dell tank
	for (int index(0); index < tank.size(); ++index) {
		if (tank[index].takeIndex() == indexTank) {
			const sf::Vector2f point = tank[index].getPosObj();
			CreateAnimBoom(point, "tankObj");
			RemovalObj(tank, index);
			break;
		}
	}
	return;
}
