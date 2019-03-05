#include "field.h"
#include "general.hpp"
#include "map.h"

void EnumerationTanks(GameField& gField)
{
	if (gField.tank.size() == 0)
		return;

	if (f_f) {
		for (auto it = gField.tank.begin(); it != gField.tank.end(); ++it) {
			if (((it - gField.tank.begin()) % 2)) //unpaired values
				it->isTank() ? (gField.*CheckOnCollision)(*it) : NULL;
		}
	}
	else {
		for (auto it = gField.tank.begin(); it != gField.tank.end(); ++it) {
			if (!((it - gField.tank.begin()) % 2)) //paired values
				it->isTank() ? (gField.*CheckOnCollision)(*it) : NULL;
		}
	}

	return;
}

void GameField::TankCollision::MonitoringCollision(GameField& gField)
{
	CollisionFrame(gField);
	CollisionBlocksSpawn(gField);
	CollisionBlocks(gField);
	CollisionTanks(gField);

	return;
}

void GameField::BulletCollision::MonitoringCollision(GameField& gField)
{
	CollisionFrame(gField);
	CollisionBlocks(gField);
	CollisionTanks(gField);
	CollisionBullets(gField);
	CollisionPlayers(gField);

	return;
}

void GameField::PlayerCollision::MonitoringCollision(GameField& gField)
{
	CollisionFrame(gField);
	CollisionBlocksSpawn(gField);
	CollisionBlocks(gField);
	CollisionTanks(gField);
	CollisionBullets(gField);
	CollisionPlayers(gField);

	return;
}

void GameField::TankCollision::CollisionFrame(GameField& gField)
{
	CheckOnCollision = &GameField::CheckOnCollisionFrame;
	EnumerationTanks(gField);
	return;
}

void GameField::TankCollision::CollisionBlocksSpawn(GameField& gField)
{
	CheckOnCollision = &GameField::CheckOnCollisionBlocksSpawn;
	EnumerationTanks(gField);
	return;
}

void GameField::TankCollision::CollisionBlocks(GameField& gField)
{
	CheckOnCollision = &GameField::CheckOnCollisionBlocks;
	EnumerationTanks(gField);
	return;
}

void GameField::TankCollision::CollisionTanks(GameField& gField)
{
	if (gField.tank.size() == 0)
		return;

	for (auto it1 = gField.tank.begin(); it1 != gField.tank.end(); ++it1) {
		for (auto it2(it1); it2 != gField.tank.end(); ++it2) {
			if (&*it1 == &*it2)
				continue;
			it1->isTank() && it2->isTank() ? gField.CheckOnCollisionTanks(*it1, *it2) : NULL;
		}
	}
	return;
}

void GameField::BulletCollision::CollisionFrame(GameField& gField)
{
	if (std::none_of(gField.bulletArr.begin(), gField.bulletArr.end(), [](Bullet* element) {return element != nullptr;}))
		return;

	const size_t bulletArrSize = gField.bulletArr.size();
	for (int indxBullet(0); indxBullet < bulletArrSize; ++indxBullet) {
		if (gField.bulletArr[indxBullet] == nullptr)
			continue;
		gField.CheckOnCollisionFrame(*gField.bulletArr[indxBullet]);
	}
}

void GameField::BulletCollision::CollisionBlocks(GameField& gField)
{
	if (std::none_of(gField.bulletArr.begin(), gField.bulletArr.end(), [](Bullet* element) {return element != nullptr;}))
		return;

	const size_t bulletArrSize = gField.bulletArr.size();
	for (int indxBullet(0); indxBullet < bulletArrSize; ++indxBullet) {
		if (gField.bulletArr[indxBullet] != nullptr) {
			gField.CheckOnCollisionBlocks(*gField.bulletArr[indxBullet]);
		}
	}
	return;
}

void GameField::BulletCollision::CollisionTanks(GameField& gField)
{
	if (std::none_of(gField.bulletArr.begin(), gField.bulletArr.end(), [](Bullet* element) {return element != nullptr;}))
		return;

	const size_t bulletArrSize = gField.bulletArr.size();
	for (int indxBullet(0); indxBullet < bulletArrSize; ++indxBullet) {
		if (gField.bulletArr[indxBullet] == nullptr)
			continue;
		gField.CheckOnCollisionTanks(*gField.bulletArr[indxBullet]);
	}
	return;
}

void GameField::BulletCollision::CollisionBullets(GameField& gField)
{
	if (std::none_of(gField.bulletArr.begin(), gField.bulletArr.end(), [](Bullet* element) {return element != nullptr;}))
		return;

	const size_t bulletArrSize = gField.bulletArr.size();
	for (int indxBullet1(0); indxBullet1 < bulletArrSize; ++indxBullet1) {
		for (int indxBullet2(0); indxBullet2 < bulletArrSize; ++indxBullet2) {
			if ((indxBullet1 == indxBullet2) || 
				(gField.bulletArr[indxBullet1] == nullptr || gField.bulletArr[indxBullet2] == nullptr))
				continue;
			gField.CheckOnCollisionBullets(*gField.bulletArr[indxBullet1], *gField.bulletArr[indxBullet2]);
		}
	}
	return;
}

void GameField::BulletCollision::CollisionPlayers(GameField& gField)
{
	if (std::none_of(gField.bulletArr.begin(), gField.bulletArr.end(), [](Bullet* element) {return element != nullptr;}))
		return;

	const size_t bulletArrSize = gField.bulletArr.size();
	for (int indxBullet(0); indxBullet < bulletArrSize; ++indxBullet) {
		if (gField.bulletArr[indxBullet] == nullptr)
			continue;
		gField.CheckOnCollisionPlayers(*gField.bulletArr[indxBullet]);
	}
	return;
}

void GameField::PlayerCollision::CollisionFrame(GameField& gField)
{
	if (gField.firstPlayer->Presence() && f_f)
		gField.CheckOnCollisionFrame(*gField.firstPlayer);
	if (gField.secondPlayer->Presence() && !f_f)
		gField.CheckOnCollisionFrame(*gField.secondPlayer);
	return;
}

void GameField::PlayerCollision::CollisionBlocksSpawn(GameField& gField)
{
	if (gField.firstPlayer->Presence() && f_f)
		gField.CheckOnCollisionBlocksSpawn(*gField.firstPlayer, 1);
	if (gField.secondPlayer->Presence() && !f_f)
		gField.CheckOnCollisionBlocksSpawn(*gField.secondPlayer, 2);
	return;
}

void GameField::PlayerCollision::CollisionBlocks(GameField& gField)
{
	if (gField.firstPlayer->Presence() && f_f)
		gField.CheckOnCollisionBlocks(*gField.firstPlayer);
	if (gField.secondPlayer->Presence() && !f_f)
		gField.CheckOnCollisionBlocks(*gField.secondPlayer);
	return;
}

void GameField::PlayerCollision::CollisionTanks(GameField& gField)
{
	if (gField.firstPlayer->Presence())
		gField.CheckOnCollisionTanks(*gField.firstPlayer);
	if (gField.secondPlayer->Presence())
		gField.CheckOnCollisionTanks(*gField.secondPlayer);
	return;
}

void GameField::PlayerCollision::CollisionBullets(GameField& gField)
{
	if (gField.firstPlayer->Presence())
		gField.CheckOnCollisionBullets(*gField.firstPlayer);
	if (gField.secondPlayer->Presence())
		gField.CheckOnCollisionBullets(*gField.secondPlayer);
	return;
}

void GameField::PlayerCollision::CollisionPlayers(GameField& gField)
{
	if (gField.firstPlayer->Presence() && gField.secondPlayer->Presence())
		gField.CheckOnCollisionPlayers(*gField.firstPlayer, *gField.secondPlayer);
	return;
}
