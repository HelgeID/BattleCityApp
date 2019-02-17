#pragma once

#include <SFML/Graphics.hpp>
#include "general.hpp"
#include "object.hpp"

#include <array>
#include <chrono>

//rnd, modification paq8n.cpp
class random
{
	std::array<unsigned long, 64> table;
	int mI{ 0 };
public:
	random()
	{
		//since epoch, nanoseconds
		unsigned long SE =
			unsigned long(std::chrono::system_clock::now().time_since_epoch() /
				std::chrono::nanoseconds(1));
		table[0] = table[1] = SE;
		for (int i = 0; i < 62; i++)
			table[i + 2] = table[i + 1] * 11 + table[i] * 23 / 16;
	}

	unsigned long operator ()()
	{
		return ++mI, table[mI & 63] = table[mI - 24 & 63] ^ table[mI - 55 & 63];
	}
};

class Bonus : public Object
{
	std::chrono::time_point<std::chrono::steady_clock> startClock, endClock;
	random rnd;
protected:
	char* type;

	Bonus(sf::Texture &texture) : Object(texture)
	{
		startClock = std::chrono::steady_clock::now();

		//set rnd position
		const float positionX((rnd() % int (SizeField - SizeCell)) + 32); // 32 ->> field shift relative to 0X
		const float positionY((rnd() % int (SizeField - SizeCell)) + 16); // 16 ->> field shift relative to 0Y
		this->setPosObj(positionX, positionY); //todo rnd Bonus
	}

	virtual ~Bonus() = 0 {}

public:
	void UpdateTimer()
	{
		endClock = std::chrono::steady_clock::now();
		auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(endClock - startClock);
		if (elapsed.count() > 200) {
			!this->isClearObj() ? this->clearSpriteObj() : this->setSpriteObj(this->xRect, this->yRect);
			startClock = std::chrono::steady_clock::now();
		}
	}

	const char* TakeType() { return type; }
};

class BonusTank : public Bonus
{
public:
	BonusTank(sf::Texture &texture) : Bonus(texture)
	{
		this->setSpriteObj(336, 112);
		this->type = "BonusTank";
	}
};

class BonusSkin : public Bonus
{
public:
	BonusSkin(sf::Texture &texture) : Bonus(texture)
	{
		this->setSpriteObj(256, 112);
		this->type = "BonusSkin";
	}
};

class BonusStar : public Bonus
{
public:
	BonusStar(sf::Texture &texture) : Bonus(texture)
	{
		this->setSpriteObj(304, 112);
		this->type = "BonusStar";
	}
};

class BonusShovel : public Bonus
{
public:
	BonusShovel(sf::Texture &texture) : Bonus(texture)
	{
		this->setSpriteObj(288, 112);
		this->type = "BonusShovel";
	}
};

class BonusClock : public Bonus
{
public:
	BonusClock(sf::Texture &texture) : Bonus(texture)
	{
		this->setSpriteObj(272, 112);
		this->type = "BonusClock";
	}
};

class BonusGrenade : public Bonus
{
public:
	BonusGrenade(sf::Texture &texture) : Bonus(texture)
	{
		this->setSpriteObj(320, 112);
		this->type = "BonusGrenade";
	}
};

class BonusPistol : public Bonus
{
public:
	BonusPistol(sf::Texture &texture) : Bonus(texture)
	{
		this->setSpriteObj(352, 112);
		this->type = "BonusPistol";
	}
};
