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
		if (tank.optTank.dir == DOWN)
			tank.loadTank(tank.optTank.col, tank.optTank.mod, UP);
		else if (tank.optTank.dir == UP)
			tank.loadTank(tank.optTank.col, tank.optTank.mod, DOWN);
		else if (tank.optTank.dir == RIGHT)
			tank.loadTank(tank.optTank.col, tank.optTank.mod, LEFT);
		else if (tank.optTank.dir == LEFT)
			tank.loadTank(tank.optTank.col, tank.optTank.mod, RIGHT);
	}
	return;
}
