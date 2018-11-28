#include "field.h"

void GameField::Collision(Tank &tank)
{
	if (tank.takeObj().getPosition().y < 0.f) {
		tank.loadTank(tank.optTank.col, tank.optTank.mod, DOWN);
	}
	else if (tank.takeObj().getPosition().y > (float)window.getSize().y - 16) {
		tank.loadTank(tank.optTank.col, tank.optTank.mod, UP);
	}
	else if (tank.takeObj().getPosition().x < 0.f) {
		tank.loadTank(tank.optTank.col, tank.optTank.mod, RIGHT);
	}
	else if (tank.takeObj().getPosition().x > (float)window.getSize().x - 16) {
		tank.loadTank(tank.optTank.col, tank.optTank.mod, LEFT);
	}
	return;
}

void GameField::Collision(Tank &tank, Tank &tank_other)
{
	if (&tank == &tank_other)
		return;

	if (tank.takeObj().getGlobalBounds().intersects(tank_other.takeObj().getGlobalBounds())) {
		Direction dirReverse;
		const Direction dirTank(tank.optTank.dir), dirTankOther(tank_other.optTank.dir);
		tank.loadTank(tank.optTank.col, tank.optTank.mod, dirReverse = tank.ReverseDirection(dirTank));
		DrawTank(tank);
		if (dirTankOther == dirReverse) {
			tank_other.loadTank(tank_other.optTank.col, tank_other.optTank.mod, dirTank);
			DrawTank(tank_other);
		}
	}
	return;
}
