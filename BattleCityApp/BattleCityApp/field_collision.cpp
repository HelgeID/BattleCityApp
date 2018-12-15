#include "field.h"
#include "general.hpp"
#include "map.h"

static const int size = 16;

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
	// old version
	//move->UP
	//	if (tank.takeObj().getPosition().y < field.getPosition().y) {
	//		tank.loadTank(tank.optTank.col, tank.optTank.mod, DOWN);
	//		tank.moveObj(0, 1);
	//		tank.mapPos.i = tank.mapPos.i - 1;
	//	}
	//move->DOWN
	//	else if (tank.takeObj().getPosition().y + 16 > field.getPosition().y + field.getSize().y) {
	//		tank.loadTank(tank.optTank.col, tank.optTank.mod, UP);
	//		tank.moveObj(0, -1);
	//		tank.mapPos.i = tank.mapPos.i + 1;
	//	}
	//move->LEFT
	//	else if (tank.takeObj().getPosition().x < field.getPosition().x) {
	//		tank.loadTank(tank.optTank.col, tank.optTank.mod, RIGHT);
	//		tank.moveObj(1, 0);
	//		tank.mapPos.j = tank.mapPos.j - 1;
	//	}
	//move->RIGHT
	//	else if (tank.takeObj().getPosition().x + 16 > field.getPosition().x + field.getSize().x) {
	//		tank.loadTank(tank.optTank.col, tank.optTank.mod, LEFT);
	//		tank.moveObj(-1, 0);
	//		tank.mapPos.j = tank.mapPos.j + 1;
	//	}
	//	return;

	// new version
	auto collisionframe = [&]
	{
		const int posX(round(tank.takeObj().getPosition().x)), posY(round(tank.takeObj().getPosition().y));
		tank.SetBoomCoord(posX, posY);
		tank.loadTank(
			tank.optTank.col,
			tank.optTank.mod,
			tank.optTank.dir = tank.RandomReverseDirection(tank.optTank.dir)
		);
		tank.setPosObj((float)posX, (float)posY);
		std::cerr << "tankDir: " << spaceTank::myDirNames[tank.optTank.dir] << std::endl;
		std::cerr << " :" << posX << " :" << posY << std::endl;

		if (!tank.CompareBoomCoord(posX, posY))
			tank.ResetBoomParam();
	};

	tank.takeObj().getPosition().y < field.getPosition().y
		||
		tank.takeObj().getPosition().y + 16 > field.getPosition().y + field.getSize().y
		||
		tank.takeObj().getPosition().x < field.getPosition().x
		||
		tank.takeObj().getPosition().x + 16 > field.getPosition().x + field.getSize().x
		? (collisionframe()) : NULL;
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

	auto Conditions = [&]() {
		int i; int j;
		int index(0);
		POS *ptr = &*arrCell;
		const int posX(round(tank.takeObj().getPosition().x)), posY(round(tank.takeObj().getPosition().y));
		for (RecShape obj : vecRecShape) {
			i = ptr->cell.y - 1; //because position of field (Y) - 16 pixels
			j = ptr->cell.x - 2; //because position of field (X) - 32 pixels
			(i >= 0) && (j >= 0) && (i < sizeMap) && (j < sizeMap) ?
				index = sizeMap * i + j : index = 0;
			if (map.GetValueMap(i, j) == 0)
				;
			else {
				if (!index)
					;
				else {
					if (p_showframe)
						window.draw(block[index].frame);
					if (tank.frame.getGlobalBounds().intersects((*(&block[0] + index)).frame.getGlobalBounds())) {
						tank.SetBoomCoord(posX, posY);
						tank.loadTank(
							tank.optTank.col,
							tank.optTank.mod,
							tank.optTank.dir = tank.RandomReverseDirection(tank.optTank.dir)
						);
						tank.setPosObj((float)posX, (float)posY);

						std::cerr << "tankDir: " << spaceTank::myDirNames[tank.optTank.dir] << std::endl;
						std::cerr << " :" << posX << " :" << posY << std::endl;
					}
					if (!tank.CompareBoomCoord(posX, posY))
						tank.ResetBoomParam();
				}
			}
			ptr++;
		}
	};

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
		if (p_showframe)
			window.draw(obj);
		ptr++;
	}

	//main conditions
	Conditions();

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

void GameField::CollisionBullets()
{
	auto create = [&](const float value)
	{
		const size_t ArrSize(8);
		auto GET_ARR = [&](int arr[], const float value)
		{
			for (size_t i(0); i < ArrSize; i++) {
				int value(fabs(value + i));
				arr[i] = value;
			}
			return arr;
		};

		int *arr = new int[ArrSize];
		return GET_ARR(arr, value);
	};

	struct PartBrickParam
	{
		float x, y;
		sf::Vector2f size;
	} objPBP;
	sf::RectangleShape frame;

	auto check_intersects = [&](const int indxBlock, const int indxBullet, const int indxNUM, const Direction dir)
	{
		objPBP.x = block[indxBlock].takePartBrick()->pbArr[indxNUM].x;
		objPBP.y = block[indxBlock].takePartBrick()->pbArr[indxNUM].y;

		block[indxBlock].takePartBrick()->pbArr[indxNUM].type == 'H' ? objPBP.size.x = 8.f, objPBP.size.y = 4.f : NULL;
		block[indxBlock].takePartBrick()->pbArr[indxNUM].type == 'V' ? objPBP.size.x = 4.f, objPBP.size.y = 8.f : NULL;

		frame.setPosition(objPBP.x, objPBP.y);
		frame.setSize(objPBP.size);

		if (block[indxBlock].takePartBrick()->pbArr[indxNUM].presence == false &&
			this->bulletArr[indxBullet]->frame.getGlobalBounds().intersects(frame.getGlobalBounds())) {
			//apply hit on the block
			{
				//directly damage
				block[indxBlock].brickDamage(partsBrickVec, indxNUM);
				pbmap.fillMap(partsBrickVec[partsBrickVec.size() - 1]);
				std::cerr << "indxNUM: " << indxNUM << std::endl;
			}
			//additional damage
			block[indxBlock].brickDamageAdditional(partsBrickVec, pbmap);

			//overload frame
			block[indxBlock].overloadFrame(dir);
			return true;
		}

		return false;
	};


	const size_t bulletArrSize = sizeof(this->bulletArr) / sizeof(*this->bulletArr);
	for (int indxBullet(0); indxBullet < bulletArrSize; ++indxBullet) {
		if (this->bulletArr[indxBullet] != nullptr) {
			int indexI = map.TakeIndex(this->bulletArr[indxBullet]->crossing_coord(), 'i');
			int indexY = map.TakeIndex(this->bulletArr[indxBullet]->crossing_coord(), 'j');
			int indxBlock = (indexI*sizeMap + indexY);
			bool crossing = (this->bulletArr[indxBullet]->frame.getGlobalBounds().intersects((*(&block[0] + indxBlock)).frame.getGlobalBounds()));
			if (crossing == true) {
				if (block[indxBlock].type == Brick) {
					int* array = nullptr;
					Direction dir = this->bulletArr[indxBullet]->takeDir();
					switch (dir)
					{
						// "|7| |0| |15| |8|" - first elements of array
					case UP:		array = create(-7.f);
						break;
					case DOWN:		array = create(0.f);
						break;
					case LEFT:		array = create(-15.f);
						break;
					case RIGHT:		array = create(8.f);
						break;
					}

					int indxPBP(0);
					while (indxPBP < 8)
					{
						int indexA = *(array + indxPBP++);
						int indexB = *(array + indxPBP++);

						if (check_intersects(indxBlock, indxBullet, indexA, dir) | check_intersects(indxBlock, indxBullet, indexB, dir)) {
							delete this->bulletArr[indxBullet];
							this->bulletArr[indxBullet] = nullptr;
							break;
						}
					}

					delete[] array;
				}
			}
		}
	}
	return;
}
