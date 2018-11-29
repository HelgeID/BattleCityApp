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

	if (tank.takeObj().getGlobalBounds().intersects(tank_other.takeObj().getGlobalBounds()))
	{
		bool r1_flag(false), r2_flag(false);
		const Direction dirTank(tank.optTank.dir), dirTankOther(tank_other.optTank.dir);

		if (((int)tank.takeObj().getPosition().x + 16 > (int)tank_other.takeObj().getPosition().x) && (dirTank == RIGHT || dirTankOther == LEFT)) {
			if (dirTankOther != LEFT && dirTank == RIGHT)
				r1_flag = true;
			else if (dirTank != RIGHT && dirTankOther == LEFT)
				r2_flag = true;
			else {
				r1_flag = true;
				r2_flag = true;
			}
		}
		else if (((int)tank.takeObj().getPosition().x < (int)tank_other.takeObj().getPosition().x + 16) && (dirTank == LEFT || dirTankOther == RIGHT)) {
			if (dirTankOther != RIGHT && dirTank == LEFT)
				r1_flag = true;
			else if (dirTank != LEFT && dirTankOther == RIGHT)
				r2_flag = true;
			else {
				r1_flag = true;
				r2_flag = true;
			}
		}
		else if (((int)tank.takeObj().getPosition().y + 16 >(int)tank_other.takeObj().getPosition().y) && (dirTank == DOWN || dirTankOther == UP)) {
			if (dirTankOther != UP && dirTank == DOWN)
				r1_flag = true;
			else if (dirTank != DOWN && dirTankOther == UP)
				r2_flag = true;
			else {
				r1_flag = true;
				r2_flag = true;
			}
		}
		else if (((int)tank.takeObj().getPosition().y < (int)tank_other.takeObj().getPosition().y + 16) && (dirTank == UP || dirTankOther == DOWN)) {
			if (dirTankOther != DOWN && dirTank == UP)
				r1_flag = true;
			else if (dirTank != UP && dirTankOther == DOWN)
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
