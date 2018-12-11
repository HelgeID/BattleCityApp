#include "field.h"
#include "general.hpp"

void GameField::Collision(Tank &tank)
{
	//move->UP
	if (tank.takeObj().getPosition().y < 8.f) {
		tank.loadTank(tank.optTank.col, tank.optTank.mod, DOWN);
		tank.mapPos.i = tank.mapPos.i - 1;
	}
	//move->DOWN
	else if (tank.takeObj().getPosition().y > (float)window.getSize().y - 24) {//16 + 8
		tank.loadTank(tank.optTank.col, tank.optTank.mod, UP);
		tank.mapPos.i = tank.mapPos.i + 1;
	}
	//move->LEFT
	else if (tank.takeObj().getPosition().x < 16.f) {
		tank.loadTank(tank.optTank.col, tank.optTank.mod, RIGHT);
		tank.mapPos.j = tank.mapPos.j - 1;
	}
	//move->RIGHT
	else if (tank.takeObj().getPosition().x > (float)window.getSize().x - 48) {//16 + 32
		tank.loadTank(tank.optTank.col, tank.optTank.mod, LEFT);
		tank.mapPos.j = tank.mapPos.j + 1;
	}
	return;
}

void GameField::Collision(Tank &tank, Tank &tank_other)
{
	if (&tank == &tank_other)
		return;

	if (tank.takeObj().getGlobalBounds().intersects(tank_other.takeObj().getGlobalBounds()))
	{
		bool r1_flag(false), r2_flag(false);
		const Direction dirTank(tank.optTank.dir), dirTankOther(tank_other.optTank.dir);

		if ((int)tank.takeObj().getPosition().x + 16 > (int)tank_other.takeObj().getPosition().x) {
			if (dirTankOther != LEFT && dirTank == RIGHT)
				r1_flag = true;
			else if (dirTank != RIGHT && dirTankOther == LEFT)
				r2_flag = true;
			else {
				r1_flag = true;
				r2_flag = true;
			}
		}

		else if ((int)tank.takeObj().getPosition().y + 16 > (int)tank_other.takeObj().getPosition().y) {
			if (dirTankOther != UP && dirTank == DOWN)
				r1_flag = true;
			else if (dirTank != DOWN && dirTankOther == UP)
				r2_flag = true;
			else {
				r1_flag = true;
				r2_flag = true;
			}
		}

		if (r1_flag == true) {
			tank.loadTank(tank.optTank.col, tank.optTank.mod, tank.ReverseDirection(dirTank));
			MoveTank(tank, 2);
		}

		if (r2_flag == true) {
			tank_other.loadTank(tank_other.optTank.col, tank_other.optTank.mod, tank.ReverseDirection(dirTankOther));
			MoveTank(tank_other, 2);
		}
	}
	return;
}

void GameField::Logic(Tank &tank)
{
	sf::Vector2f currentTankPos = {
		tank.takeObj().getPosition().x,
		tank.takeObj().getPosition().y
	};

	sf::Vector2f currentMapPos = map.TakeCoord(
		tank.mapPos.i,
		tank.mapPos.j
	);

	auto conditionsLess = [&](Map& map, float& value1, float& value2, int& value3) {
		DrawTank(tank);
		if (value1 >= value2 - 16)
			;
		else {
			map.SetValueMap(0, currentMapPos);
			value3--;
		}
	};

	auto conditionsMore = [&](Map& map, float& value1, float& value2, int& value3) {
		DrawTank(tank);
		if (value1 <= value2 + 16)
			;
		else {
			map.SetValueMap(0, currentMapPos);
			value3++;
		}
	};

	if (tank.optTank.dir == UP) {
		if (!map.GetNextUpValueMap(tank.mapPos.i, tank.mapPos.j)) {
			map.SetValueMap(100, tank.mapPos.i - 1, tank.mapPos.j);
		}
		else {
			conditionsLess(map, currentTankPos.y, currentMapPos.y, tank.mapPos.i);
		}
	}
	else if (tank.optTank.dir == LEFT) {
		if (!map.GetNextLeftValueMap(tank.mapPos.i, tank.mapPos.j)) {
			map.SetValueMap(100, tank.mapPos.i, tank.mapPos.j - 1);
		}
		else {
			conditionsLess(map, currentTankPos.x, currentMapPos.x, tank.mapPos.j);
		}
	}
	else if (tank.optTank.dir == DOWN) {
		if (!map.GetNextDownValueMap(tank.mapPos.i, tank.mapPos.j)) {
			map.SetValueMap(100, tank.mapPos.i + 1, tank.mapPos.j);
		}
		else {
			conditionsMore(map, currentTankPos.y, currentMapPos.y, tank.mapPos.i);
		}
	}
	else if (tank.optTank.dir == RIGHT) {
		if (!map.GetNextRightValueMap(tank.mapPos.i, tank.mapPos.j)) {
			map.SetValueMap(100, tank.mapPos.i, tank.mapPos.j + 1);
		}
		else {
			conditionsMore(map, currentTankPos.x, currentMapPos.x, tank.mapPos.j);
		}
	}

}
