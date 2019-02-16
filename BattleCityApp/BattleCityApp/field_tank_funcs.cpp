#include "field.h"
#include "general.hpp"
#include <thread>

void GameField::CreateTanks()
{
	if (p_player == 1)
		for (int i = 0; i < 4; i++) //max tank == 4, when one player
			CreateTank(sf::Vector2f(0.f, 0.f));
	if (p_player == 2)
		for (int i = 0; i < 6; i++) //max tank == 6, when two players
			CreateTank(sf::Vector2f(0.f, 0.f));

	//std::unique_ptr<std::thread> thread_control(new std::thread(&LAUNCHING_TANKS, this));
	//thread_control->detach();
	return;
}

void GameField::CreateTank(const sf::Vector2f pos)
{
	static Tank tankObj(texture);
	tank.push_back(tankObj);
	(tank.end() - 1)->loadTank(RED, modA, DOWN, false);
	(tank.end() - 1)->loadIndex(tank);
	(tank.end() - 1)->setPosObj(pos.x, pos.y);
	return;
}

void GameField::ReloadTank(Tank& tank, const sf::Vector2f pos)
{
	tank.loadTank(WHITE, modA, DOWN, true);
	tank.setPosObj(pos.x, pos.y);
	return;
}

void GameField::MoveTank(Tank& tank, float position)
{
	switch (tank.optTank.dir)
	{
		case UP: MoveTank(UP, tank, position); break;
		case LEFT: MoveTank(LEFT, tank, position); break;
		case DOWN: MoveTank(DOWN, tank, position); break;
		case RIGHT: MoveTank(RIGHT, tank, position); break;
	}
	return;
}

void GameField::MoveTank(const Direction dir, Tank& tank, float position)
{
	if (dir == UP)
		tank.moveObj(0.f, -position);
	else if (dir == LEFT)
		tank.moveObj(-position, 0.f);
	else if (dir == DOWN)
		tank.moveObj(0.f, position);
	else if (dir == RIGHT)
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

	//off tank
	for (int index(0); index < tank.size(); ++index) {
		if (tank[index].takeIndex() == indexTank) {
			if (tank[index].optTank.bonus) {
				tank[index].optTank.bonus = !tank[index].optTank.bonus;
				tank[index].loadTank(Color::WHITE, 
					tank[index].optTank.mod,
					tank[index].optTank.dir,
					tank[index].optTank.bonus
				);
				break;
			}

			const sf::Vector2f point = tank[index].getPosObj();
			CreateAnimBoom(point, "tankObj");
			tank[index].offTank();

			//init pos to NULL
			tank[index].setPosObj(0.f, 0.f);
			tank[index].setPosFrame(tank[index].getPosObj().x, tank[index].getPosObj().y);

			//launching a new tank on the field
			std::unique_ptr<std::thread> thread_control(new std::thread(&LOAD_TANK, this, false));
			thread_control->detach();
			break;
		}
	}
	return;
}
