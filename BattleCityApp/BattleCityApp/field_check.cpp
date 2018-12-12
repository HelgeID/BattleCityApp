#include "field.h"
#include "general.hpp"

#define size 16

class RecShape : public sf::RectangleShape
{
public:
	RecShape()
	{
		this->setSize(sf::Vector2f(size, size));
		this->setFillColor(sf::Color(127, 127, 127, 64));
	}
};

std::vector<RecShape> vecRecShape;

void GameField::CollisionFrame(Tank &tank)
{
	//move->UP
	if (tank.takeObj().getPosition().y < field.getPosition().y) {
		tank.loadTank(tank.optTank.col, tank.optTank.mod, DOWN);
		tank.moveObj(0, 1);
		tank.mapPos.i = tank.mapPos.i - 1;
	}
	//move->DOWN
	else if (tank.takeObj().getPosition().y + 16 > field.getPosition().y + field.getSize().y) {
		tank.loadTank(tank.optTank.col, tank.optTank.mod, UP);
		tank.moveObj(0, -1);
		tank.mapPos.i = tank.mapPos.i + 1;
	}
	//move->LEFT
	else if (tank.takeObj().getPosition().x < field.getPosition().x) {
		tank.loadTank(tank.optTank.col, tank.optTank.mod, RIGHT);
		tank.moveObj(1, 0);
		tank.mapPos.j = tank.mapPos.j - 1;
	}
	//move->RIGHT
	else if (tank.takeObj().getPosition().x + 16 > field.getPosition().x + field.getSize().x) {
		tank.loadTank(tank.optTank.col, tank.optTank.mod, LEFT);
		tank.moveObj(-1, 0);
		tank.mapPos.j = tank.mapPos.j + 1;
	}
	return;
}

void GameField::CollisionBlocks(Tank &tank)
{
	struct POS { sf::Vector2i cell; } *arrCell(nullptr);
	RecShape recShape;

	const int posX((int)tank.frame.getPosition().x);
	const int posY((int)tank.frame.getPosition().y);

	int posCenterX = posX + (size / 2);
	int posCenterY = posY + (size / 2);
	int posMatrixX = ((posX / size) + 1) * size;
	int posMatrixY = ((posY / size) + 1) * size;

	auto TakeCell = [&](sf::Vector2i& cell, const int offsetX, const int offsetY)
	{
		cell = sf::Vector2i(posMatrixX / size + offsetX, posMatrixY / size + offsetY);
		vecRecShape.push_back(recShape);
	};

	auto TakeCellDef = [&](POS arrCell[], const size_t length)
	{
		TakeCell(arrCell[length - 4].cell, -1, -1);
		TakeCell(arrCell[length - 3].cell, 0, -1);
		TakeCell(arrCell[length - 2].cell, -1, 0);
		TakeCell(arrCell[length - 1].cell, 0, 0);
	};

	if (posCenterX > posMatrixX && posCenterY > posMatrixY)
	{
		const size_t length(9);
		arrCell = new POS[length];
		TakeCell(arrCell[0].cell, 1, -1);
		TakeCell(arrCell[1].cell, 1, 0);
		TakeCell(arrCell[2].cell, 1, 1);
		TakeCell(arrCell[3].cell, 0, 1);
		TakeCell(arrCell[4].cell, -1, 1);
		TakeCellDef(arrCell, length);
	}
	else if (posCenterX > posMatrixX && posCenterY < posMatrixY)
	{
		const size_t length(9);
		arrCell = new POS[length];
		TakeCell(arrCell[0].cell, -1, -2);
		TakeCell(arrCell[1].cell, 0, -2);
		TakeCell(arrCell[2].cell, 1, -2);
		TakeCell(arrCell[3].cell, 1, -1);
		TakeCell(arrCell[4].cell, 1, 0);
		TakeCellDef(arrCell, length);
	}
	else if (posCenterX < posMatrixX && posCenterY < posMatrixY)
	{
		const size_t length(9);
		arrCell = new POS[length];
		TakeCell(arrCell[0].cell, -2, 0);
		TakeCell(arrCell[1].cell, -2, -1);
		TakeCell(arrCell[2].cell, -2, -2);
		TakeCell(arrCell[3].cell, -1, -2);
		TakeCell(arrCell[4].cell, 0, -2);
		TakeCellDef(arrCell, length);
	}
	else if (posCenterX < posMatrixX && posCenterY > posMatrixY)
	{
		const size_t length(9);
		arrCell = new POS[length];
		TakeCell(arrCell[0].cell, 0, 1);
		TakeCell(arrCell[1].cell, -1, 1);
		TakeCell(arrCell[2].cell, -2, 1);
		TakeCell(arrCell[3].cell, -2, 0);
		TakeCell(arrCell[4].cell, -2, -1);
		TakeCellDef(arrCell, length);
	}
	else if (posCenterX > posMatrixX && posCenterY == posMatrixY)
	{
		const size_t length(6);
		arrCell = new POS[length];
		TakeCell(arrCell[0].cell, 1, -1);
		TakeCell(arrCell[1].cell, 1, 0);
		TakeCellDef(arrCell, length);
	}
	else if (posCenterX < posMatrixX && posCenterY == posMatrixY)
	{
		const size_t length(6);
		arrCell = new POS[length];
		TakeCell(arrCell[0].cell, -2, -1);
		TakeCell(arrCell[1].cell, -2, 0);
		TakeCellDef(arrCell, length);
	}
	else if (posCenterX == posMatrixX && posCenterY > posMatrixY)
	{
		const size_t length(6);
		arrCell = new POS[length];
		TakeCell(arrCell[0].cell, 0, 1);
		TakeCell(arrCell[1].cell, -1, 1);
		TakeCellDef(arrCell, length);
	}
	else if (posCenterX == posMatrixX && posCenterY < posMatrixY)
	{
		const size_t length(6);
		arrCell = new POS[length];
		TakeCell(arrCell[0].cell, 0, -2);
		TakeCell(arrCell[1].cell, -1, -2);
		TakeCellDef(arrCell, length);
	}
	else
	{
		const size_t length(4);
		arrCell = new POS[length];
		TakeCellDef(arrCell, length);
	}

	POS *ptr = &*arrCell;
	for (RecShape obj : vecRecShape) {
		obj.setPosition((float)ptr->cell.x * size, (float)ptr->cell.y * size);
		window.draw(obj);
		ptr++;
	}

	delete[] arrCell;
	vecRecShape.clear();
	return;
}

void GameField::CollisionTanks(Tank &tank, Tank &tank_other)
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
