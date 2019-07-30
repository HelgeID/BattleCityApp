//FINAL
#pragma once

#include <SFML/Graphics.hpp>
#include "general.hpp"
#include "object.hpp"

#include <array>
#include <chrono>

//rnd, modification paq8n.cpp
class MyRandom
{
	std::array<unsigned long, 64> table;
	int mI{ 0 };
public:
	MyRandom()
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

inline sf::Vector2f SearchCoord(sf::Texture texture, const sf::Vector2f pos1, const sf::Vector2f pos2)
{
	MyRandom rnd;
	bool flg1 = false;
	bool flg2 = false;
	//int count(0);
	sf::Vector2f pos;
	do
	{
		//std::cerr << "SearchCoord count:" << count++ << std::endl;
		pos.x = ((rnd() % int(SizeField - SizeCell)) + 32); // 32 ->> field shift relative to 0X
		pos.y = ((rnd() % int(SizeField - SizeCell)) + 16); // 16 ->> field shift relative to 0Y
		Object objPL1(texture); objPL1.setPosObj(pos1.x, pos1.y);
		Object objPL2(texture); objPL2.setPosObj(pos2.x, pos2.y);
		Object objBonus(texture); objBonus.setPosObj(pos.x, pos.y);
		flg1 = (objPL1.takeObj().getGlobalBounds().intersects(objBonus.takeObj().getGlobalBounds()));
		flg2 = (objPL2.takeObj().getGlobalBounds().intersects(objBonus.takeObj().getGlobalBounds()));
	} while (flg1 || flg2);
	return pos;
}

class Bonus : public Object
{
	std::chrono::time_point<std::chrono::steady_clock> startClock, endClock;
protected:
	char* type;

	Bonus(sf::Texture &texture, const sf::Vector2f pos1, const sf::Vector2f pos2) : Object(texture)
	{
		startClock = std::chrono::steady_clock::now();

		sf::Vector2f pos(SearchCoord(texture, pos1, pos2));
		this->setPosObj(pos.x, pos.y);

		//todo rnd Bonus ...
	}

public:
	virtual ~Bonus() = 0 {}

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
	BonusTank(sf::Texture &texture, const sf::Vector2f pos1, const sf::Vector2f pos2) : Bonus(texture, pos1, pos2)
	{
		this->setSpriteObj(336, 112);
		this->type = "BonusTank";
	}
};

class BonusSkin : public Bonus
{
public:
	BonusSkin(sf::Texture &texture, const sf::Vector2f pos1, const sf::Vector2f pos2) : Bonus(texture, pos1, pos2)
	{
		this->setSpriteObj(256, 112);
		this->type = "BonusSkin";
	}
};

class BonusStar : public Bonus
{
public:
	BonusStar(sf::Texture &texture, const sf::Vector2f pos1, const sf::Vector2f pos2) : Bonus(texture, pos1, pos2)
	{
		this->setSpriteObj(304, 112);
		this->type = "BonusStar";
	}
};

class BonusShovel : public Bonus
{
public:
	BonusShovel(sf::Texture &texture, const sf::Vector2f pos1, const sf::Vector2f pos2) : Bonus(texture, pos1, pos2)
	{
		this->setSpriteObj(288, 112);
		this->type = "BonusShovel";
	}
};

class BonusClock : public Bonus
{
public:
	BonusClock(sf::Texture &texture, const sf::Vector2f pos1, const sf::Vector2f pos2) : Bonus(texture, pos1, pos2)
	{
		this->setSpriteObj(272, 112);
		this->type = "BonusClock";
	}
};

class BonusGrenade : public Bonus
{
public:
	BonusGrenade(sf::Texture &texture, const sf::Vector2f pos1, const sf::Vector2f pos2) : Bonus(texture, pos1, pos2)
	{
		this->setSpriteObj(320, 112);
		this->type = "BonusGrenade";
	}
};

class BonusPistol : public Bonus
{
public:
	BonusPistol(sf::Texture &texture, const sf::Vector2f pos1, const sf::Vector2f pos2) : Bonus(texture, pos1, pos2)
	{
		this->setSpriteObj(352, 112);
		this->type = "BonusPistol";
	}
};
