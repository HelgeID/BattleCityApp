#include "field.h"
#include "general.hpp"
#include "map.h"

//////////////////////////////////////////////////
//additional functionality for CollisionBlocks()
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
//////////////////////////////////////////////////

void GameField::TankCollision::MonitoringCollision(GameField& gField)
{
	//coef_reload
	gField.coef_reload = gField.coef_reload + 1;
	gField.coef_reload > 6 ? gField.coef_reload = 0 : 0; //todo max_coef_reload

	CollisionFrame(gField);
	CollisionBlocks(gField);
	CollisionTanks(gField);

	return;
}

void GameField::TankCollision::CollisionFrame(GameField& gField)
{
	for (auto it = gField.tank.begin(); it != gField.tank.end(); ++it)
	{
		auto collisionframe = [&]
		{
			const int posX(round(it->takeObj().getPosition().x)), posY(round(it->takeObj().getPosition().y));
			const bool bulletActivFlag(it->optTankShooting.bulletActivFlag);
			it->SetBoomCoord(posX, posY);
			it->loadTank(
				it->optTank.col,
				it->optTank.mod,
				it->optTank.dir = it->RandomReverseDirection(it->optTank.dir)
			);
			it->optTankShooting.bulletActivFlag = bulletActivFlag;
			it->setPosObj((float)posX, (float)posY);
			std::cerr << "tankDir: " << spaceTank::myDirNames[it->optTank.dir] << std::endl;
			std::cerr << " :" << posX << " :" << posY << std::endl;

			it->ResetBoomParam();
		};

		it->takeObj().getPosition().y < gField.field.getPosition().y
			||
			it->takeObj().getPosition().y + 16 > gField.field.getPosition().y + gField.field.getSize().y
			||
			it->takeObj().getPosition().x < gField.field.getPosition().x
			||
			it->takeObj().getPosition().x + 16 > gField.field.getPosition().x + gField.field.getSize().x
			? (collisionframe()) : NULL;

		gField.ControlTank_onFrame(*it);
	}
	return;
}

void GameField::TankCollision::CollisionBlocks(GameField& gField)
{
	for (auto it = gField.tank.begin(); it != gField.tank.end(); ++it)
	{
		struct POS { sf::Vector2i cell; } *arrCell(nullptr);
		RecShape recShape;

		const int posX((int)it->frame.getPosition().x);
		const int posY((int)it->frame.getPosition().y);

		int posCenterX = posX + (size / 2);
		int posCenterY = posY + (size / 2);
		int posMatrixX = ((posX / size) + 1) * size;
		int posMatrixY = ((posY / size) + 1) * size;

		auto Conditions = [&]() {
			int i; int j;
			int index(0);
			POS *ptr = &*arrCell;
			const int posX(round(it->takeObj().getPosition().x)), posY(round(it->takeObj().getPosition().y));
			for (RecShape obj : vecRecShape) {
				i = ptr->cell.y - 1; //because position of field (Y) - 16 pixels
				j = ptr->cell.x - 2; //because position of field (X) - 32 pixels
				(i >= 0) && (j >= 0) && (i < sizeMap) && (j < sizeMap) ?
					index = sizeMap * i + j : index = 0;
				if (gField.map.GetValueMap(i, j) == 0)
					;
				else {
					if (!index)
						;
					else {
						if (p_showframe)
							gField.window.draw(gField.block[index].frame);
						if (it->frame.getGlobalBounds().intersects((*(&gField.block[0] + index)).frame.getGlobalBounds())) {
							const bool bulletActivFlag(it->optTankShooting.bulletActivFlag);
							it->SetBoomCoord(posX, posY);
							it->loadTank(
								it->optTank.col,
								it->optTank.mod,
								it->optTank.dir = it->RandomReverseDirection(it->optTank.dir)
							);
							it->optTankShooting.bulletActivFlag = bulletActivFlag;
							it->setPosObj((float)posX, (float)posY);

							std::cerr << "tankDir: " << spaceTank::myDirNames[it->optTank.dir] << std::endl;
							std::cerr << " :" << posX << " :" << posY << std::endl;
						}
						if (!it->CompareBoomCoord(posX, posY))
							it->ResetBoomParam();
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
				gField.window.draw(obj);
			ptr++;
		}

		Conditions();

		delete[] arrCell;
		vecRecShape.clear();
	}
	return;
}

void GameField::TankCollision::CollisionTanks(GameField& gField)
{
	for (auto it1 = gField.tank.begin(); it1 != gField.tank.end(); ++it1) {
		for (auto it2(it1); it2 != gField.tank.end(); ++it2) {
			if (&*it1 == &*it2)
				continue;

			if (it1->takeObj().getGlobalBounds().intersects(it2->takeObj().getGlobalBounds()))
			{
				bool r1_flag(false), r2_flag(false);
				const Direction dirTank(it1->optTank.dir), dirTankOther(it2->optTank.dir);

				if ((int)it1->takeObj().getPosition().x + 16 > (int)it2->takeObj().getPosition().x) {
					if (dirTankOther != LEFT && dirTank == RIGHT)
						r1_flag = true;
					else if (dirTank != RIGHT && dirTankOther == LEFT)
						r2_flag = true;
					else {
						r1_flag = true;
						r2_flag = true;
					}
				}

				else if ((int)it1->takeObj().getPosition().y + 16 > (int)it2->takeObj().getPosition().y) {
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
					const bool bulletActivFlag(it1->optTankShooting.bulletActivFlag);
					it1->loadTank(it1->optTank.col, it1->optTank.mod, it1->ReverseDirection(dirTank));
					it1->optTankShooting.bulletActivFlag = bulletActivFlag;
					gField.MoveTank(*it1, 2);
				}

				if (r2_flag == true) {
					const bool bulletActivFlag(it2->optTankShooting.bulletActivFlag);
					it2->loadTank(it2->optTank.col, it2->optTank.mod, it1->ReverseDirection(dirTankOther));
					it2->optTankShooting.bulletActivFlag = bulletActivFlag;
					gField.MoveTank(*it2, 2);
				}
			}
		}
	}
}
