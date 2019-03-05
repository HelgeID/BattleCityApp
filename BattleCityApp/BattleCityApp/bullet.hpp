#pragma once

#include <SFML/Graphics.hpp>
#include "general.hpp"
#include "object.hpp"
#include "frame.hpp"

#define BulletSpeed 500

class Bullet : public Object, public Frame
{
	sf::Vector2f size(const Direction &direction, const float offsetX, const float offsetY)
	{
		//direction value:
		//UP(0), LEFT(1), DOWN(2), RIGHT(3)

		//offset:
		//by default - 0px, frame - 2px

		//size of object
		if (direction % 2)
			return sf::Vector2f(4.f - offsetX, 3.f - offsetY);
		else
			return sf::Vector2f(3.f - offsetX, 4.f - offsetY);
	}

	Direction dir;

public:
	Bullet(sf::Texture &texture, sf::Vector2f pos, const Direction dir) : Object(texture, size(dir, 0.f, 0.f)), Frame("bullet", size(dir, 0.f, 0.f)),
		dir(dir)
	{
		if (dir == UP) { this->setSpriteObj(323, 102); goto exit; }
		if (dir == LEFT) { this->setSpriteObj(330, 102); goto exit; }
		if (dir == DOWN) { this->setSpriteObj(339, 102); goto exit; }
		if (dir == RIGHT) { this->setSpriteObj(346, 102); goto exit; }

	exit: ;

		this->setPosObj(pos.x, pos.y);
	}

	bool* bulletActivFlag;
	int indexTank; //who owns the bullet
	int indxBullet; //position in the array 

	void move(const float time = 10)
	{
		float x, y;
		if (dir == UP) { x = 0.f; y = -0.1f*time; goto exit; }
		if (dir == LEFT) { x = -0.1f*time; y = 0.f; goto exit; }
		if (dir == DOWN) { x = 0.f; y = 0.1f*time; goto exit; }
		if (dir == RIGHT) { x = 0.1f*time; y = 0.f; goto exit; }

	exit: ;

		moveObj(x, y);
		return;
	}

	sf::Vector2f crossing_coord() const
	{
		const float x = this->getPosObj().x + this->getSizeObj().x / 2;
		const float y = this->getPosObj().y + this->getSizeObj().y / 2;
		return sf::Vector2f(x, y);
	}

	Direction takeDir() const { return dir; }
};
