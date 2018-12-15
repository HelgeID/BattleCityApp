﻿#include "field.h"
#include "general.hpp"

//accessory functions for CreateBullet FUNC
Bullet* _createBulletUP(const sf::Vector2f, sf::Texture&);
Bullet* _createBulletLEFT(const sf::Vector2f, sf::Texture&);
Bullet* _createBulletDOWN(const sf::Vector2f, sf::Texture&);
Bullet* _createBulletRIGHT(const sf::Vector2f, sf::Texture&);

void GameField::CreateBullet(Tank& tank, sf::Vector2f offset = sf::Vector2f(0.f, 0.f))
{
	const Direction dir(tank.optTank.dir);
	sf::Vector2f point; //central point of the sprite 
	point.x = tank.getPosObj().x + tank.getSizeObj().x / 2 + offset.x;
	point.y = tank.getPosObj().y + tank.getSizeObj().y / 2 + offset.y;

	Bullet *bulletObj(nullptr);
	switch (dir) {
		case UP:  bulletObj = _createBulletUP(point, texture); break;
		case LEFT: bulletObj = _createBulletLEFT(point, texture); break;
		case DOWN: bulletObj = _createBulletDOWN(point, texture); break;
		case RIGHT: bulletObj = _createBulletRIGHT(point, texture); break;
	}

	const size_t bulletArrSize = sizeof(this->bulletArr) / sizeof(*this->bulletArr);
	for (int i(0); i < bulletArrSize; ++i) {
		if (this->bulletArr[i] == nullptr) {
			this->bulletArr[i] = bulletObj;
			break;
		}
	}
	return;
}

void GameField::CreateBullet(Tank& tank) { CreateBullet(tank, sf::Vector2f(0.f, 0.f)); }

Bullet* _createBulletUP(const sf::Vector2f point, sf::Texture &texture)
{
	float pX = point.x;
	float pY = point.y - 8;
	return (new Bullet(texture, sf::Vector2f(pX-1, pY), UP));
}

Bullet* _createBulletLEFT(const sf::Vector2f point, sf::Texture &texture)
{
	float pX = point.x - 8;
	float pY = point.y;
	return (new Bullet(texture, sf::Vector2f(pX, pY-1), LEFT));
}

Bullet* _createBulletDOWN(const sf::Vector2f point, sf::Texture &texture)
{
	float pX = point.x;
	float pY = point.y + 8;
	return (new Bullet(texture, sf::Vector2f(pX-1, pY), DOWN));
}

Bullet* _createBulletRIGHT(const sf::Vector2f point, sf::Texture &texture)
{
	float pX = point.x + 8;
	float pY = point.y;
	return (new Bullet(texture, sf::Vector2f(pX, pY-1), RIGHT));
}