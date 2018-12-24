#include "field.h"
#include "general.hpp"
#include "map.h"

void GameField::BulletCollision::MonitoringCollision(GameField& gField)
{
	CollisionFrame(gField);
	CollisionBlocks(gField);
	CollisionTanks(gField);
	CollisionBullets(gField);

	return;
}

void GameField::BulletCollision::CollisionFrame(GameField& gField)
{
	const size_t bulletArrSize = sizeof(gField.bulletArr) / sizeof(*gField.bulletArr);
	for (int indxBullet(0); indxBullet < bulletArrSize; ++indxBullet) {
		if (gField.bulletArr[indxBullet] == nullptr)
			continue;

		auto collisionframe = [&]
		{
			//say the tank that the bullet hit the target
			*gField.bulletArr[indxBullet]->bulletActivFlag = false;
			//remove the bullet
			delete gField.bulletArr[indxBullet];
			gField.bulletArr[indxBullet] = nullptr;
		};

		gField.bulletArr[indxBullet]->takeObj().getPosition().y < gField.field.getPosition().y
			||
			gField.bulletArr[indxBullet]->takeObj().getPosition().y + 4 > gField.field.getPosition().y + gField.field.getSize().y
			||
			gField.bulletArr[indxBullet]->takeObj().getPosition().x < gField.field.getPosition().x
			||
			gField.bulletArr[indxBullet]->takeObj().getPosition().x + 4 > gField.field.getPosition().x + gField.field.getSize().x
			? (collisionframe()) : NULL;
	}
}

void GameField::BulletCollision::CollisionBlocks(GameField& gField)
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
		objPBP.x = gField.block[indxBlock].takePartBrick()->pbArr[indxNUM].x;
		objPBP.y = gField.block[indxBlock].takePartBrick()->pbArr[indxNUM].y;

		gField.block[indxBlock].takePartBrick()->pbArr[indxNUM].type == 'H' ? objPBP.size.x = 8.f, objPBP.size.y = 4.f : NULL;
		gField.block[indxBlock].takePartBrick()->pbArr[indxNUM].type == 'V' ? objPBP.size.x = 4.f, objPBP.size.y = 8.f : NULL;

		frame.setPosition(objPBP.x, objPBP.y);
		frame.setSize(objPBP.size);

		if (gField.block[indxBlock].takePartBrick()->pbArr[indxNUM].presence == false &&
			gField.bulletArr[indxBullet]->frame.getGlobalBounds().intersects(frame.getGlobalBounds())) {
			//apply hit on the block
				{
					//directly damage
					gField.block[indxBlock].brickDamage(gField.partsBrickVec, indxNUM);
					gField.pbmap.fillMap(gField.partsBrickVec[gField.partsBrickVec.size() - 1]);
					std::cerr << "indxNUM: " << indxNUM << std::endl;
				}
				//additional damage
				//gField.block[indxBlock].brickDamageAdditional(gField.partsBrickVec, gField.pbmap);

				//overload frame
				gField.block[indxBlock].overloadFrame(dir);
				return true;
		}

		return false;
	};

	const size_t bulletArrSize = sizeof(gField.bulletArr) / sizeof(*gField.bulletArr);
	for (int indxBullet(0); indxBullet < bulletArrSize; ++indxBullet) {
		if (gField.bulletArr[indxBullet] != nullptr) {
			int indexI = gField.map.TakeIndex(gField.bulletArr[indxBullet]->crossing_coord(), 'i');
			int indexY = gField.map.TakeIndex(gField.bulletArr[indxBullet]->crossing_coord(), 'j');
			int indxBlock = (indexI*sizeMap + indexY);
			bool crossing = (gField.bulletArr[indxBullet]->frame.getGlobalBounds().intersects((*(&gField.block[0] + indxBlock)).frame.getGlobalBounds()));
			if (crossing == true) {
				if (gField.block[indxBlock].type == Brick) {
					int* array = nullptr;
					Direction dir = gField.bulletArr[indxBullet]->takeDir();
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
							//say the tank that the bullet hit the target
							*gField.bulletArr[indxBullet]->bulletActivFlag = false;
							//remove the bullet
							delete gField.bulletArr[indxBullet];
							gField.bulletArr[indxBullet] = nullptr;
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

void GameField::BulletCollision::CollisionTanks(GameField& gField)
{
	const size_t bulletArrSize = sizeof(gField.bulletArr) / sizeof(*gField.bulletArr);
	for (int indxBullet(0); indxBullet < bulletArrSize; ++indxBullet) {
		if (gField.bulletArr[indxBullet] == nullptr)
			return;
		for (auto it = gField.tank.begin(); it != gField.tank.end(); ++it) {
			if (gField.bulletArr[indxBullet]->frame.getGlobalBounds().intersects(it->frame.getGlobalBounds())) {
				if (gField.bulletArr[indxBullet]->indexTank == it->takeIndex())
					continue;
				if (gField.bulletArr[indxBullet]->indexTank == 100 || gField.bulletArr[indxBullet]->indexTank == 200)
					; //todo

				//say the tank that the bullet hit the target
				*gField.bulletArr[indxBullet]->bulletActivFlag = false;
				//remove the bullet
				delete gField.bulletArr[indxBullet];
				gField.bulletArr[indxBullet] = nullptr;
				break;
			}
		}
	}
	return;
}

void GameField::BulletCollision::CollisionBullets(GameField& gField)
{
	const size_t bulletArrSize = sizeof(gField.bulletArr) / sizeof(*gField.bulletArr);
	for (int indxBullet1(0); indxBullet1 < bulletArrSize; ++indxBullet1) {
		for (int indxBullet2(0); indxBullet2 < bulletArrSize; ++indxBullet2) {
			if (indxBullet1 == indxBullet2)
				continue;

			if (gField.bulletArr[indxBullet1] != nullptr && gField.bulletArr[indxBullet2] != nullptr)
				if (gField.bulletArr[indxBullet1]->frame.getGlobalBounds().intersects(gField.bulletArr[indxBullet2]->frame.getGlobalBounds())) {
					//say the tanks that the bullet hit the target
					*gField.bulletArr[indxBullet1]->bulletActivFlag = false; *gField.bulletArr[indxBullet2]->bulletActivFlag = false;
					//remove the bullets
					delete gField.bulletArr[indxBullet1]; gField.bulletArr[indxBullet1] = nullptr;
					delete gField.bulletArr[indxBullet2]; gField.bulletArr[indxBullet2] = nullptr;
				}
		}
	}
	return;
}

void GameField::BulletCollision::CollisionActor1(GameField& gField)
{

}

void GameField::BulletCollision::CollisionActor2(GameField& gField)
{

}
