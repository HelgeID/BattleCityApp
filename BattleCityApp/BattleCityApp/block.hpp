//FINAL
#pragma once

#include <SFML/Graphics.hpp>
#include <memory>
#include <iostream>
#include "general.hpp"
#include "object.hpp"
#include "frame.hpp"

#include "part_bricks_map.h"

enum Environment { Empty, Brick, Steel, Trees, Water, Ice }; //block type

enum ModBlock { Nothing, modRight, modDown, modLeft, modUp, modRightUpMini, modRightDownMini, modLeftUpMini, modLeftDownMini }; //offset block

//constants for determining the ordinal position in a texture (0 - 24), (25 - 49), ...
// level[index] = value + const; // in LoadMap()
#define EmptyConst  ""
#define BrickConst  6
#define SteelConst  21
#define TreesConst  37
#define WaterConst  26
#define IceConst    18

namespace spaceBlock
{
	struct MapPosition
	{
		int i, j;
	};
}

//PartBrick
struct PartBrick
{
	float x, y;
	int type; //uses: 'H' & 'V'
	bool presence;
};

class PartsBrickArr
{
public:
	PartBrick pbArr[16];
	bool pbStateArr[8] = { false, false, false, false, false, false, false, false };
	std::unique_ptr<bool> pbStateL{ new bool(false) };
	std::unique_ptr<bool> pbStateR{ new bool(false) };
	std::unique_ptr<bool> pbStateU{ new bool(false) };
	std::unique_ptr<bool> pbStateD{ new bool(false) };
};

class Block : public Object, public Frame
{
	std::shared_ptr<PartsBrickArr> partBrick;
public:
	const std::shared_ptr<PartsBrickArr>& takePartBrick() { return partBrick; }
	void loadParamPartsBrick(std::vector<sf::RectangleShape>&, Part_Bricks_Map& );
	void brickDamage(std::vector<sf::RectangleShape>&, const int);
	void brickDamageAdditional(std::vector<sf::RectangleShape>&, Part_Bricks_Map& );
	void overloadFrame(const Direction);

private:
	Environment TakeTape(const int& value)
	{
		return Environment(value / 10);
	}

	ModBlock TakeMod(const int& value)
	{
		if (!value)
			return Nothing;

		if (value == 11 || value == 21)
			return modRight;
		else if (value == 12 || value == 22)
			return modDown;
		else if (value == 13 || value == 23)
			return modLeft;
		else if (value == 14 || value == 24)
			return modUp;

		else if (value == 1114 || value == 2124)
			return modRightUpMini;
		else if (value == 1112 || value == 2122)
			return modRightDownMini;
		else if (value == 1314 || value == 2324)
			return modLeftUpMini;
		else if (value == 1312 || value == 2322)
			return modLeftDownMini;

		else
			return Nothing;
	}

	struct COORD { int x; int y; };

	void BrickCoord(COORD& coord, const int value)
	{
		coord.x = value % 10 * 16 + 256; coord.y = 0;
		return;
	}

	void SteelCoord(COORD& coord, const int value)
	{
		coord.x = value % 20 * 16 + 256; coord.y = 16;
		return;
	}

	void TreesCoord(COORD& coord, const int value)
	{
		if (value != 30)
			return;
		coord.x = 272; coord.y = 32;
		return;
	}

	void WaterCoord(COORD& coord, const int value)
	{
		if (value != 40)
			return;
		coord.x = 256; coord.y = 32;
		return;
	}

	void IceCoord(COORD& coord, const int value)
	{
		if (value != 50)
			return;
		coord.x = 288; coord.y = 32;
		return;
	}

public:
	Block(sf::Texture &texture) : Object(texture), Frame("block")
	{
	}

	Block& operator=(const Block& obj)
	{
		if (this == &obj)
			return *this;
		this->~Block();
		new (this) Block(obj);
		return *this;
	}

	Environment type;
	ModBlock mod;

	void loadBlock(int value = 0)
	{
		this->mod = TakeMod(value);

		if (value > 999) {
			(value / 1000 == 1) ? this->type = Brick :
				(value / 1000 == 2) ? this->type = Steel : NULL;

			{ value /= 1000; value *= 10; } //lead to the whole block (10 || 20), so two actions
		}
		else
			this->type = TakeTape(value);

		COORD *coord = new COORD;

		switch (type) {
			case Empty: break;
			case Brick: BrickCoord(*coord, value); break;
			case Steel: SteelCoord(*coord, value); break;
			case Trees: TreesCoord(*coord, value); break;
			case Water: WaterCoord(*coord, value); break;
			case Ice: IceCoord(*coord, value); break;

		default: break;
		}
		this->setSpriteObj(coord->x, coord->y);
		delete coord;
	}

	void drawBlock(sf::RenderWindow &window)
	{
		window.draw(this->takeObj());
		return;
	}

};
