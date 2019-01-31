#include "field.h"
#include "general.hpp"
#include "map.h"

/////////////////////////////////////////////////////////////////////////////
//ENEMIE
/////////////////////////////////////////////////////////////////////////////

// Checking the tank on a collision with the frame
void GameField::CheckOnCollisionFrame(Tank& tank)
{
	auto collisionframe = [&]
	{
		const int posX(round(tank.takeObj().getPosition().x)), posY(round(tank.takeObj().getPosition().y));
		const bool bulletActivFlag(tank.optTankShooting.bulletActivFlag);
		tank.SetBoomCoord(posX, posY);
		tank.loadTank(
			tank.optTank.col,
			tank.optTank.mod,
			tank.optTank.dir = tank.RandomReverseDirection(tank.optTank.dir)
		);
		tank.optTankShooting.bulletActivFlag = bulletActivFlag;
		tank.setPosObj((float)posX, (float)posY);
		std::cerr << "tankDir: " << spaceTank::myDirNames[tank.optTank.dir] << std::endl;
		std::cerr << " :" << posX << " :" << posY << std::endl;

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

	//todo
	ControlTank_onFrame(tank);

	return;
}

// Checking the tank on a collision with the block
template<typename T>
void GameField::FormArrayCells(T **arrCell, const sf::Vector2i centerPos, const sf::Vector2i matrixPos, RecShape& recShape)
{
	auto TakeCell = [&](sf::Vector2i& cell, const int offsetX, const int offsetY)
	{
		cell = sf::Vector2i(matrixPos.x / SizeCell + offsetX, matrixPos.y / SizeCell + offsetY);
		vecRecShape.push_back(recShape);
	};

	auto TakeCellDef = [&](T arrCell[], const size_t length)
	{
		TakeCell(arrCell[length - 4].cell, -1, -1);
		TakeCell(arrCell[length - 3].cell, 0, -1);
		TakeCell(arrCell[length - 2].cell, -1, 0);
		TakeCell(arrCell[length - 1].cell, 0, 0);
	};

	if (centerPos.x > matrixPos.x && centerPos.y > matrixPos.y)
	{
		const size_t length(9);
		*arrCell = new T[length];
		TakeCell((*arrCell + 0)->cell, 1, -1);
		TakeCell((*arrCell + 1)->cell, 1, 0);
		TakeCell((*arrCell + 2)->cell, 1, 1);
		TakeCell((*arrCell + 3)->cell, 0, 1);
		TakeCell((*arrCell + 4)->cell, -1, 1);
		TakeCellDef(*arrCell, length);
	}
	else if (centerPos.x > matrixPos.x && centerPos.y < matrixPos.y)
	{
		const size_t length(9);
		*arrCell = new T[length];
		TakeCell((*arrCell + 0)->cell, -1, -2);
		TakeCell((*arrCell + 1)->cell, 0, -2);
		TakeCell((*arrCell + 2)->cell, 1, -2);
		TakeCell((*arrCell + 3)->cell, 1, -1);
		TakeCell((*arrCell + 4)->cell, 1, 0);
		TakeCellDef(*arrCell, length);
	}
	else if (centerPos.x < matrixPos.x && centerPos.y < matrixPos.y)
	{
		const size_t length(9);
		*arrCell = new T[length];
		TakeCell((*arrCell + 0)->cell, -2, 0);
		TakeCell((*arrCell + 1)->cell, -2, -1);
		TakeCell((*arrCell + 2)->cell, -2, -2);
		TakeCell((*arrCell + 3)->cell, -1, -2);
		TakeCell((*arrCell + 4)->cell, 0, -2);
		TakeCellDef(*arrCell, length);
	}
	else if (centerPos.x < matrixPos.x && centerPos.y > matrixPos.y)
	{
		const size_t length(9);
		*arrCell = new T[length];
		TakeCell((*arrCell + 0)->cell, 0, 1);
		TakeCell((*arrCell + 1)->cell, -1, 1);
		TakeCell((*arrCell + 2)->cell, -2, 1);
		TakeCell((*arrCell + 3)->cell, -2, 0);
		TakeCell((*arrCell + 4)->cell, -2, -1);
		TakeCellDef(*arrCell, length);
	}
	else if (centerPos.x > matrixPos.x && centerPos.y == matrixPos.y)
	{
		const size_t length(6);
		*arrCell = new T[length];
		TakeCell((*arrCell + 0)->cell, 1, -1);
		TakeCell((*arrCell + 1)->cell, 1, 0);
		TakeCellDef(*arrCell, length);
	}
	else if (centerPos.x < matrixPos.x && centerPos.y == matrixPos.y)
	{
		const size_t length(6);
		*arrCell = new T[length];
		TakeCell((*arrCell + 0)->cell, -2, -1);
		TakeCell((*arrCell + 1)->cell, -2, 0);
		TakeCellDef(*arrCell, length);
	}
	else if (centerPos.x == matrixPos.x && centerPos.y > matrixPos.y)
	{
		const size_t length(6);
		*arrCell = new T[length];
		TakeCell((*arrCell + 0)->cell, 0, 1);
		TakeCell((*arrCell + 1)->cell, -1, 1);
		TakeCellDef(*arrCell, length);
	}
	else if (centerPos.x == matrixPos.x && centerPos.y < matrixPos.y)
	{
		const size_t length(6);
		*arrCell = new T[length];
		TakeCell((*arrCell + 0)->cell, 0, -2);
		TakeCell((*arrCell + 1)->cell, -1, -2);
		TakeCellDef(*arrCell, length);
	}
	else
	{
		const size_t length(4);
		*arrCell = new T[length];
		TakeCellDef(*arrCell, length);
	}
}

template<typename T>
void GameField::DisbandArrayCells(T **arrCell)
{
	delete[] * arrCell;
	vecRecShape.clear();
}

void GameField::CheckOnCollisionBlocksSpawn(Tank& tank)
{
	auto ReverseTank = [&]
	{
		const bool bulletActivFlag(tank.optTankShooting.bulletActivFlag);
		tank.loadTank(tank.optTank.col, tank.optTank.mod, tank.ReverseDirection(tank.optTank.dir));
		tank.optTankShooting.bulletActivFlag = bulletActivFlag;
		MoveTank(tank, 2);
		tank.setPosFrame(tank.takeObj().getPosition().x, tank.takeObj().getPosition().y);
	};

	if (l_BS->Spawn() == true)
		if (tank.takeObj().getGlobalBounds().intersects(l_BS->takeObj().getGlobalBounds()))
			ReverseTank();

	if (r_BS->Spawn() == true)
		if (tank.takeObj().getGlobalBounds().intersects(r_BS->takeObj().getGlobalBounds()))
			ReverseTank();

	if (c_BS->Spawn() == true)
		if (tank.takeObj().getGlobalBounds().intersects(c_BS->takeObj().getGlobalBounds()))
			ReverseTank();

	return;
}

void GameField::CheckOnCollisionBlocks(Tank& tank, const bool fPL)
{
	struct POS { sf::Vector2i cell; } *arrCell(nullptr);
	RecShape recShape;

	sf::Vector2i centerPos, matrixPos;
	const int posX((int)tank.frame.getPosition().x);
	const int posY((int)tank.frame.getPosition().y);
	centerPos.x = posX + (SizeCell / 2);
	centerPos.y = posY + (SizeCell / 2);
	matrixPos.x = ((posX / SizeCell) + 1) * SizeCell;
	matrixPos.y = ((posY / SizeCell) + 1) * SizeCell;

	auto CollisionBlocks = [&]() {
		int i; int j;
		int indxBlock(0);
		POS *ptr = &*arrCell;
		const int posX(round(tank.takeObj().getPosition().x)), posY(round(tank.takeObj().getPosition().y));
		for (RecShape obj : vecRecShape) {
			i = ptr->cell.y - 1; //because position of field (Y) - 16 pixels
			j = ptr->cell.x - 2; //because position of field (X) - 32 pixels
			(i >= 0) && (j >= 0) && (i < sizeMap) && (j < sizeMap) ?
				indxBlock = sizeMap * i + j : indxBlock = 0;
			if (map.GetValueMap(i, j) == 0)
				;
			else {
				if (!indxBlock)
					;
				else {
					if (p_showframe)
						window.draw(block[indxBlock].frame);

					auto crossing = [&](int index) {
						if (!fPL)
							return tank.frame.getGlobalBounds().intersects((*(&block[0] + index)).frame.getGlobalBounds()); // for enemie
						return tank.frame.getGlobalBounds().intersects((*(&block[0] + index)).frame.getGlobalBounds()); // for player
					};

					if (block[indxBlock].type == Brick || block[indxBlock].type == Steel)
					{
						if (crossing(indxBlock)) {
							//processing player
							if (fPL == true) {
								while (crossing(indxBlock)) {
									MoveTank(tank, -1.f);
									tank.setPosFrame(tank.takeObj().getPosition().x, tank.takeObj().getPosition().y);
								}
							}
							//processing enemie
							else
							{
								Direction dirlast;
								const bool bulletActivFlag(tank.optTankShooting.bulletActivFlag);
								tank.SetBoomCoord(posX, posY);
								tank.loadTank(
									tank.optTank.col,
									tank.optTank.mod,
									tank.optTank.dir = tank.RandomReverseDirection(dirlast = tank.optTank.dir)
								);
								tank.optTankShooting.bulletActivFlag = bulletActivFlag;
								tank.setPosObj((float)posX, (float)posY);
								MoveTank(dirlast, tank, -1.f);
								tank.setPosFrame(tank.takeObj().getPosition().x, tank.takeObj().getPosition().y);

								std::cerr << "tankDir: " << spaceTank::myDirNames[tank.optTank.dir] << std::endl;
								std::cerr << " :" << posX << " :" << posY << std::endl;
							}
						}
						if (!tank.CompareBoomCoord(posX, posY) && !fPL)
							tank.ResetBoomParam();
					}

					if (block[indxBlock].type == Trees) {
						p_showframe ? window.draw(block[indxBlock].frame) :
							block[indxBlock].drawBlock(window);
					}
					//todo type
				}
			}
			ptr++;
		}
	};

	FormArrayCells<POS>(&arrCell,
		centerPos,
		matrixPos,
		recShape
		);

	POS *ptr = &*arrCell;
	for (RecShape obj : vecRecShape) {
		obj.setPosition((float)ptr->cell.x * SizeCell, (float)ptr->cell.y * SizeCell);
		if (p_showframe)
			window.draw(obj);
		ptr++;
	}

	CollisionBlocks();
	DisbandArrayCells<POS>(&arrCell);
	return;
}

// Checking the tank on a collision with the other tank
void GameField::CheckOnCollisionTanks(Tank& tank1, Tank& tank2)
{
	if (tank1.takeObj().getGlobalBounds().intersects(tank2.takeObj().getGlobalBounds()))
	{
		bool r1_flag(false), r2_flag(false);
		const Direction dirTank(tank1.optTank.dir), dirTankOther(tank2.optTank.dir);

		if ((int)tank1.takeObj().getPosition().x + 16 > (int)tank2.takeObj().getPosition().x) {
			if (dirTankOther != LEFT && dirTank == RIGHT)
				r1_flag = true;
			else if (dirTank != RIGHT && dirTankOther == LEFT)
				r2_flag = true;
			else {
				r1_flag = true;
				r2_flag = true;
			}
		}

		else if ((int)tank1.takeObj().getPosition().y + 16 > (int)tank2.takeObj().getPosition().y) {
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
			const bool bulletActivFlag(tank1.optTankShooting.bulletActivFlag);
			tank1.loadTank(tank1.optTank.col, tank1.optTank.mod, tank1.ReverseDirection(dirTank));
			tank1.optTankShooting.bulletActivFlag = bulletActivFlag;
			MoveTank(tank1, 2);
			tank1.setPosFrame(tank1.takeObj().getPosition().x, tank1.takeObj().getPosition().y);
		}

		if (r2_flag == true) {
			const bool bulletActivFlag(tank2.optTankShooting.bulletActivFlag);
			tank2.loadTank(tank2.optTank.col, tank2.optTank.mod, tank1.ReverseDirection(dirTankOther));
			tank2.optTankShooting.bulletActivFlag = bulletActivFlag;
			MoveTank(tank2, 2);
			tank2.setPosFrame(tank2.takeObj().getPosition().x, tank2.takeObj().getPosition().y);
		}
	}
	return;
}

/////////////////////////////////////////////////////////////////////////////
//Bullet
/////////////////////////////////////////////////////////////////////////////

// Checking the bullet on a collision with the frame
void GameField::CheckOnCollisionFrame(Bullet& bullet)
{
	const int indxBullet = bullet.indxBullet;

	auto CollisionFrame = [&]
	{
		//create anim
		sf::Vector2f point(
			bulletArr[indxBullet]->takeObj().getPosition().x,
			bulletArr[indxBullet]->takeObj().getPosition().y
		);
		point.x = point.x - 8;
		point.y = point.y - 8;
		CreateAnimBoom(point, "bulletObj");

		//say the tank that the bullet hit the target
		*bulletArr[indxBullet]->bulletActivFlag = false;
		//remove the bullet
		delete bulletArr[indxBullet];
		bulletArr[indxBullet] = nullptr;
	};

	bulletArr[indxBullet]->takeObj().getPosition().y < field.getPosition().y
		||
		bulletArr[indxBullet]->takeObj().getPosition().y + 4 > field.getPosition().y + field.getSize().y
		||
		bulletArr[indxBullet]->takeObj().getPosition().x < field.getPosition().x
		||
		bulletArr[indxBullet]->takeObj().getPosition().x + 4 > field.getPosition().x + field.getSize().x
		? (CollisionFrame()) : NULL;
	return;
}

// Checking the bullet on a collision with the block
void GameField::CheckOnCollisionBlocks(Bullet& bullet)
{
	const int indxBullet = bullet.indxBullet;

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
			bulletArr[indxBullet]->frame.getGlobalBounds().intersects(frame.getGlobalBounds())) {
			//apply hit on the block
				{
					//directly damage
					block[indxBlock].brickDamage(partsBrickVec, indxNUM);
					pbmap.fillMap(partsBrickVec[partsBrickVec.size() - 1]);
				}
				//additional damage
				block[indxBlock].brickDamageAdditional(partsBrickVec, pbmap);

				//overload frame
				block[indxBlock].overloadFrame(dir);
				return true;
		}

		return false;
	};

	int indexI = map.TakeIndex(bulletArr[indxBullet]->crossing_coord(), 'i');
	int indexY = map.TakeIndex(bulletArr[indxBullet]->crossing_coord(), 'j');
	int indxBlock = (indexI*sizeMap + indexY);
	bool crossing = (bulletArr[indxBullet]->frame.getGlobalBounds().intersects((*(&block[0] + indxBlock)).frame.getGlobalBounds()));
	if (crossing == true) {
		if (block[indxBlock].type == Brick) {
			int* array = nullptr;
			Direction dir = bulletArr[indxBullet]->takeDir();
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
					//create anim
					sf::Vector2f point(
						bulletArr[indxBullet]->takeObj().getPosition().x,
						bulletArr[indxBullet]->takeObj().getPosition().y
					);
					point.x = point.x - 8;
					point.y = point.y - 8;
					CreateAnimBoom(point, "bulletObj");

					//say the tank that the bullet hit the target
					*bulletArr[indxBullet]->bulletActivFlag = false;
					//remove the bullet
					delete bulletArr[indxBullet];
					bulletArr[indxBullet] = nullptr;
					break;
				}
			}

			delete[] array;
		}

		if (block[indxBlock].type == Steel) {
			sf::Vector2f point(
				bulletArr[indxBullet]->takeObj().getPosition().x,
				bulletArr[indxBullet]->takeObj().getPosition().y
			);
			point.x = point.x - 8;
			point.y = point.y - 8;
			CreateAnimBoom(point, "bulletObj");

			//say the tank that the bullet hit the target
			*bulletArr[indxBullet]->bulletActivFlag = false;
			//remove the bullet
			delete bulletArr[indxBullet];
			bulletArr[indxBullet] = nullptr;
		}
	}
	return;
}

// Checking the bullet on a collision with the tank
void GameField::CheckOnCollisionTanks(Bullet& bullet)
{
	const int indxBullet = bullet.indxBullet;

	for (auto it = tank.begin(); it != tank.end(); ++it) {
		if (!it->isTank())
			continue;

		if (bulletArr[indxBullet]->frame.getGlobalBounds().intersects(it->frame.getGlobalBounds())) {
			if (bulletArr[indxBullet]->indexTank == it->takeIndex())
				continue;
			if (bulletArr[indxBullet]->indexTank == 100 || bulletArr[indxBullet]->indexTank == 200) {
				const int indexTank(it->takeIndex());
				CheckTankBang(indexTank);
			}

			//create anim
			sf::Vector2f point(
				bulletArr[indxBullet]->takeObj().getPosition().x,
				bulletArr[indxBullet]->takeObj().getPosition().y
			);
			point.x = point.x - 8;
			point.y = point.y - 8;
			CreateAnimBoom(point, "bulletObj");

			//say the tank that the bullet hit the target
			*bulletArr[indxBullet]->bulletActivFlag = false;
			//remove the bullet
			delete bulletArr[indxBullet];
			bulletArr[indxBullet] = nullptr;
			break;
		}
	}
	return;
}

// Checking the bullet on a collision with the other bullet
void GameField::CheckOnCollisionBullets(Bullet& bullet1, Bullet& bullet2)
{
	const int indxBullet1 = bullet1.indxBullet;
	const int indxBullet2 = bullet2.indxBullet;

	if (bulletArr[indxBullet1] != nullptr && bulletArr[indxBullet2] != nullptr)
		if (bulletArr[indxBullet1]->frame.getGlobalBounds().intersects(bulletArr[indxBullet2]->frame.getGlobalBounds())) {
			//create anim1, anim2
			sf::Vector2f point1(
				bulletArr[indxBullet1]->takeObj().getPosition().x,
				bulletArr[indxBullet1]->takeObj().getPosition().y
			);
			point1.x = point1.x - 8;
			point1.y = point1.y - 8;
			CreateAnimBoom(point1, "bulletObj");

			sf::Vector2f point2(
				bulletArr[indxBullet2]->takeObj().getPosition().x,
				bulletArr[indxBullet2]->takeObj().getPosition().y
			);
			point2.x = point2.x - 8;
			point2.y = point2.y - 8;
			CreateAnimBoom(point2, "bulletObj");

			//say the tanks that the bullet hit the target
			*bulletArr[indxBullet1]->bulletActivFlag = false; *bulletArr[indxBullet2]->bulletActivFlag = false;
			//remove the bullets
			delete bulletArr[indxBullet1]; bulletArr[indxBullet1] = nullptr;
			delete bulletArr[indxBullet2]; bulletArr[indxBullet2] = nullptr;
		}
	return;
}

// Checking the bullet on a collision with the players
void GameField::CheckOnCollisionPlayers(Bullet& bullet)
{
	const int indxBullet = bullet.indxBullet;
	const sf::FloatRect bullet_globalBounds(bulletArr[indxBullet]->frame.getGlobalBounds());
	sf::FloatRect player_globalBounds;
	bool PlayerFirstBoomFLAG(false), PlayerSecondBoomFLAG(false);

	if (firstPlayer->Presence() && bullet.indexTank != 100) {
		if (bullet.indexTank == 200 && undying_hit_on_player)
			return;
		player_globalBounds = firstPlayer->takeObj().getGlobalBounds();
		PlayerFirstBoomFLAG = bullet_globalBounds.intersects(player_globalBounds);
	}

	if (secondPlayer->Presence() && bullet.indexTank != 200) {
		if (bullet.indexTank == 100 && undying_hit_on_player)
			return;
		player_globalBounds = secondPlayer->takeObj().getGlobalBounds();
		PlayerSecondBoomFLAG = bullet_globalBounds.intersects(player_globalBounds);
	}

	if (PlayerFirstBoomFLAG || PlayerSecondBoomFLAG)
	{
		//create anim
		sf::Vector2f point(
			bulletArr[indxBullet]->takeObj().getPosition().x,
			bulletArr[indxBullet]->takeObj().getPosition().y
		);
		point.x = point.x - 8;
		point.y = point.y - 8;
		CreateAnimBoom(point, "bulletObj");

		//say the tank that the bullet hit the target
		*bulletArr[indxBullet]->bulletActivFlag = false;
		//remove the bullet
		delete bulletArr[indxBullet];
		bulletArr[indxBullet] = nullptr;
	}

	PlayerFirstBoomFLAG ? CheckPlayerBang(*firstPlayer) : NULL;
	PlayerSecondBoomFLAG ? CheckPlayerBang(*secondPlayer) : NULL;

	return;
}

/////////////////////////////////////////////////////////////////////////////
//PLAYERs
/////////////////////////////////////////////////////////////////////////////

// Checking FirstPlayer & SecondPlayer
auto PLAYER_BOOM = [](Tank& tank1, Tank& tank2)
{
	sf::FloatRect bulletRectPlayer1 = tank1.takeObj().getGlobalBounds();
	sf::FloatRect bulletRectPlayer2 = tank2.takeObj().getGlobalBounds();

	const bool fX(bulletRectPlayer1.left > bulletRectPlayer2.left);
	const bool fY(bulletRectPlayer1.top > bulletRectPlayer2.top);

	if (tank2.optTank.dir == UP) {
		if (tank1.optTank.dir == UP) {
			fY == true ? tank1.moveObj(0.f, 1.f) :
				tank2.moveObj(0.f, 1.f);
		}
		else if (tank1.optTank.dir == LEFT) {
			fX == true ? tank1.moveObj(1.f, 0.f) :
				!fY ? tank2.moveObj(0.f, 1.f) : NULL;
		}
		else if (tank1.optTank.dir == DOWN) {
			!fY ? tank1.moveObj(0.f, -1.f),
				tank2.moveObj(0.f, 1.f) : NULL;
		}
		else if (tank1.optTank.dir == RIGHT) {
			!fX ? tank1.moveObj(-1.f, 0.f) :
				!fY ? tank2.moveObj(0.f, 1.f) : NULL;
		}
	}
	else if (tank2.optTank.dir == LEFT) {
		if (tank1.optTank.dir == UP) {
			fY == true ? tank1.moveObj(0.f, 1.f) :
				!fX ? tank2.moveObj(1.f, 0.f) : NULL;
		}
		else if (tank1.optTank.dir == LEFT) {
			fX == true ? tank1.moveObj(1.f, 0.f) :
				tank2.moveObj(1.f, 0.f);
		}
		else if (tank1.optTank.dir == DOWN) {
			!fY ? tank1.moveObj(0.f, -1.f) :
				!fX ? tank2.moveObj(1.f, 0.f) : NULL;
		}
		else if (tank1.optTank.dir == RIGHT) {
			!fX ? tank1.moveObj(-1.f, 0.f),
				tank2.moveObj(1.f, 0.f) : NULL;
		}
	}
	else if (tank2.optTank.dir == DOWN) {
		if (tank1.optTank.dir == UP) {
			fY == true ? tank1.moveObj(0.f, 1.f),
				tank2.moveObj(0.f, -1.f) : NULL;
		}
		else if (tank1.optTank.dir == LEFT) {
			fX == true ? tank1.moveObj(1.f, 0.f) :
				fY == true ? tank2.moveObj(0.f, -1.f) : NULL;
		}
		else if (tank1.optTank.dir == DOWN) {
			!fY ? tank1.moveObj(0.f, -1.f) :
				tank2.moveObj(0.f, -1.f);
		}
		else if (tank1.optTank.dir == RIGHT) {
			!fX ? tank1.moveObj(-1.f, 0.f) :
				fY == true ? tank2.moveObj(0.f, -1.f) : NULL;
		}
	}
	else if (tank2.optTank.dir == RIGHT) {
		if (tank1.optTank.dir == UP) {
			fY == true ? tank1.moveObj(0.f, 1.f) :
				fX == true ? tank2.moveObj(-1.f, 0.f) : NULL;
		}
		else if (tank1.optTank.dir == LEFT) {
			fX == true ? tank1.moveObj(1.f, 0.f),
				tank2.moveObj(-1.f, 0.f) : NULL;
		}
		else if (tank1.optTank.dir == DOWN) {
			!fY ? tank1.moveObj(0.f, -1.f) :
				fX == true ? tank2.moveObj(-1.f, 0.f) : NULL;
		}
		else if (tank1.optTank.dir == RIGHT) {
			!fX ? tank1.moveObj(-1.f, 0.f) :
				tank2.moveObj(-1.f, 0.f);
		}
	}
};

// Checking the player on a collision with the frame
void GameField::CheckOnCollisionFrame(Player& player)
{
	auto relocate = [&](Player& player)
	{
		if (player.takeObj().getGlobalBounds().intersects(outsideUP.getGlobalBounds()))
			player.moveObj(0.f, 1.f);
		else if (player.takeObj().getGlobalBounds().intersects(outsideDOWN.getGlobalBounds()))
			player.moveObj(0.f, -1.f);
		else if (player.takeObj().getGlobalBounds().intersects(outsideLEFT.getGlobalBounds()))
			player.moveObj(1.f, 0.f);
		else if (player.takeObj().getGlobalBounds().intersects(outsideRIGHT.getGlobalBounds()))
			player.moveObj(-1.f, 0.f);
	};

	relocate(player);
	return;
}

// Checking the player on a collision with the block spawn
void GameField::CheckOnCollisionBlocksSpawn(Player& player, const int num)
{
	bool flag(false);
	if (l_BS->Spawn() == true) {
		if (player.takeObj().getGlobalBounds().intersects(l_BS->takeObj().getGlobalBounds())) {
			CheckPlayerBang(player);
			flag = !flag;
		}
	}
	if (r_BS->Spawn() == true) {
		if (player.takeObj().getGlobalBounds().intersects(r_BS->takeObj().getGlobalBounds())) {
			CheckPlayerBang(player);
			flag = !flag;
		}
	}
	if (c_BS->Spawn() == true) {
		if (player.takeObj().getGlobalBounds().intersects(c_BS->takeObj().getGlobalBounds())) {
			CheckPlayerBang(player);
			flag = !flag;
		}
	}

	if (flag) {
		if (num == 1 && firstPlayerAnim.playerSkin != nullptr) {
			firstPlayerAnim.playerSkin.reset();
			firstPlayerAnim.playerSkin = nullptr;
			player.SkinOff();
		}
		else if (num == 2 && secondPlayerAnim.playerSkin != nullptr) {
			secondPlayerAnim.playerSkin.reset();
			secondPlayerAnim.playerSkin = nullptr;
			player.SkinOff();
		}
	}
	return;
}

// Checking the player on a collision with the block
void GameField::CheckOnCollisionBlocks(Player& player)
{
	CheckOnCollisionBlocks(player, true);
	return;
}

// Checking the player on a collision with the tank
void GameField::CheckOnCollisionTanks(Player& player)
{
	auto rotation = [&](Tank& tank)
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
		tank.ResetBoomParam();
	};

	for (auto it = tank.begin(); it != tank.end(); ++it) {
		if (!it->isTank())
			continue;
		if (player.takeObj().getGlobalBounds().intersects(it->takeObj().getGlobalBounds())) {
			PLAYER_BOOM(player, *it);
			if ((player.optTank.dir == UP && it->optTank.dir == DOWN) ||
				(player.optTank.dir == LEFT && it->optTank.dir == RIGHT) ||
				(player.optTank.dir == DOWN && it->optTank.dir == UP) ||
				(player.optTank.dir == RIGHT && it->optTank.dir == LEFT))
				//rotation(*it);
				CheckPlayerBang(player, true);
		}
	}		
	return;
}

// Checking the player on a collision with the bullets (not using)
void GameField::CheckOnCollisionBullets(Player& player)
{
	return;
}

// Checking the player on a collision with the other player
void GameField::CheckOnCollisionPlayers(Player& player1, Player& player2)
{
	if (player1.takeObj().getGlobalBounds().intersects(player2.takeObj().getGlobalBounds())) {
		(Key_A || Key_D || Key_W || Key_S) ?
			PLAYER_BOOM(player1, player2) : NULL;
		(Key_Left || Key_Right || Key_Up || Key_Down) ?
			PLAYER_BOOM(player2, player1) : NULL;
	}
	return;
}

/////////////////////////////////////////////////////////////////////////////
//Moore
/////////////////////////////////////////////////////////////////////////////
void GameField::CheckOnMoore()
{
	bool removal(false);

	if (moore.size() > 0)
		;
	else
		return;

	//Check the players, enemies

	bool fPL(false);
	auto crossing = [&](Block& block, Tank& tank) {
		if (!fPL)
			return tank.frame.getGlobalBounds().intersects(block.frame.getGlobalBounds()); // for enemie
		return tank.frame.getGlobalBounds().intersects(block.frame.getGlobalBounds()); // for player
	};

	auto PlAYER_BOOM = [&](Block& block, Tank& tank)
	{
		//processing player
		while (crossing(block, tank))
		{
			MoveTank(tank, -1.f);
			tank.setPosFrame(tank.takeObj().getPosition().x, tank.takeObj().getPosition().y);
		}
	};

	auto BOOM = [&](Block& block, Tank& tank, const sf::Vector2i pos)
	{
		//processing enemie
		Direction dirlast;
		const bool bulletActivFlag(tank.optTankShooting.bulletActivFlag);
		tank.SetBoomCoord(pos.x, pos.y);
		tank.loadTank(
			tank.optTank.col,
			tank.optTank.mod,
			tank.optTank.dir = tank.RandomReverseDirection(dirlast = tank.optTank.dir)
		);
		tank.optTankShooting.bulletActivFlag = bulletActivFlag;
		tank.setPosObj((float)pos.x, (float)pos.y);
		MoveTank(dirlast, tank, -1.f);
		tank.setPosFrame(tank.takeObj().getPosition().x, tank.takeObj().getPosition().y);

		std::cerr << "tankDir: " << spaceTank::myDirNames[tank.optTank.dir] << std::endl;
		std::cerr << " :" << pos.x << " :" << pos.y << std::endl;
	};

	auto Compare = [&](Tank& tank, const sf::Vector2i pos)
	{
		if (!tank.CompareBoomCoord(pos.x, pos.y))
			tank.ResetBoomParam();
	};

	for (size_t iMoore(0); iMoore < moore.size(); ++iMoore)
	{
		fPL = false; //signal => the enemies
		if (tank.size() != 0) {
			for (size_t iTank(0); iTank < tank.size(); ++iTank) {
				if (tank[iTank].isTank()) {
					const sf::Vector2i posTank(
						round(tank[iTank].takeObj().getPosition().x), 
						round(tank[iTank].takeObj().getPosition().y)
					);
					if (crossing(moore[iMoore], tank[iTank]))
						BOOM(moore[iMoore], tank[iTank], posTank);
					Compare(tank[iTank], posTank);
				}
			}
		}

		fPL = true; //signal => the player

		if (firstPlayer->Presence()) {
			if (crossing(moore[iMoore], *firstPlayer))
				PlAYER_BOOM(moore[iMoore], *firstPlayer);
		}

		if (secondPlayer->Presence()) {
			if (crossing(moore[iMoore], *secondPlayer))
				PlAYER_BOOM(moore[iMoore], *secondPlayer);
		}
	}

	//Check the bullets

	if (std::none_of(bulletArr.begin(), bulletArr.end(), [](Bullet* element) {return element != nullptr;}))
		return;

	const size_t bulletArrSize = bulletArr.size();
	for (int indxBullet(0); indxBullet < bulletArrSize; ++indxBullet) {
		if (bulletArr[indxBullet] != nullptr && moore.size() > 0) {
			for (size_t iMoore(0); iMoore < moore.size(); ++iMoore) {
				bool crossing = (bulletArr[indxBullet]->frame.getGlobalBounds().intersects(moore[iMoore].frame.getGlobalBounds()));
				if (crossing) {
					sf::Vector2f point(
						bulletArr[indxBullet]->takeObj().getPosition().x,
						bulletArr[indxBullet]->takeObj().getPosition().y
					);
					point.x = point.x - 8;
					point.y = point.y - 8;
					CreateAnimBoom(point, "bulletObj");

					//say the tank that the bullet hit the target
					*bulletArr[indxBullet]->bulletActivFlag = false;
					//remove the bullet
					delete bulletArr[indxBullet];
					bulletArr[indxBullet] = nullptr;

					//remove the block
					if (moore[iMoore].type == Brick)
						RemovalObj(moore, iMoore);

					break;
				}
			}
		}
	}
	return;
}
