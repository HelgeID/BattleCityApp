#include "field.h"
#include "general.hpp"

//accessory functions for CreateBullet FUNC
Bullet* _createBulletUP(const sf::Vector2f, sf::Texture&);
Bullet* _createBulletLEFT(const sf::Vector2f, sf::Texture&);
Bullet* _createBulletDOWN(const sf::Vector2f, sf::Texture&);
Bullet* _createBulletRIGHT(const sf::Vector2f, sf::Texture&);

template <typename T>
void GameField::CreateBullet(T& tank, sf::Vector2f offset = sf::Vector2f(0.f, 0.f))
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
	
	tank.optTankShooting.bulletActivFlag = true;
	bulletObj->bulletActivFlag = &tank.optTankShooting.bulletActivFlag;
	bulletObj->indexTank = tank.takeIndex();

	const size_t bulletArrSize = this->bulletArr.size();
	for (int indxBullet(0); indxBullet < bulletArrSize; ++indxBullet) {
		if (this->bulletArr[indxBullet] == nullptr) {
			bulletObj->indxBullet = indxBullet;
			this->bulletArr[indxBullet] = bulletObj;
			break;
		}
	}

	return;
}

void GameField::CreateBullet(Tank& tank) { CreateBullet<Tank>(tank, sf::Vector2f(0.f, 0.f)); }
void GameField::CreateBullet(Player& player) { CreateBullet<Player>(player, sf::Vector2f(0.f, 0.f)); }

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

void GameField::updBullets()
{
	float speed = BulletSpeed;
	float time = this->time / speed;

	const size_t bulletArrSize = this->bulletArr.size();
	for (size_t indexBullet(0); indexBullet < bulletArrSize; ++indexBullet) {
		if (this->bulletArr[indexBullet] != nullptr) {
			this->bulletArr[indexBullet]->move(time);
			this->bulletArr[indexBullet]->setPosFrame(
				this->bulletArr[indexBullet]->takeObj().getPosition().x,
				this->bulletArr[indexBullet]->takeObj().getPosition().y
			);
		}
	}
	return;
}
