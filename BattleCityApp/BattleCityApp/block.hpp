#pragma once

#include <SFML/Graphics.hpp>
#include <memory>
#include <iostream>
#include "general.hpp"
#include "object.hpp"
#include "frame.hpp"

#include "part_bricks_map.h"

enum Environment { Empty, Brick, Steel, Trees, Water, Ice };

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
	void loadParamPartsBrick();
	void brickDamage(std::vector<sf::RectangleShape>&, const int);
	void brickDamageAdditional(std::vector<sf::RectangleShape>&, Part_Bricks_Map& );
	void overloadFrame(const Direction);

private:
	Environment TakeTape(const int& value)
	{
		if (value / 10 == 1)
			return Brick;
		else if (value / 20 == 1)
			return Steel;
		else if (value / 30 == 1)
			return Trees;
		else if (value / 40 == 1)
			return Water;
		else if (value / 50 == 1)
			return Ice;
		return Empty;
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

public:
	Block(sf::Texture &texture) : Object(texture), Frame("block")
	{
	}

	Environment type;
	void loadBlock(int value = 0)
	{
		this->type = TakeTape(value);

		COORD *coord = new COORD;

		switch (type) {
			case Empty: break;
			case Brick: BrickCoord(*coord, value); break;
			case Steel: SteelCoord(*coord, value); break;
			case Trees: break; //todo
			case Water: break; //todo
			case Ice: break; //todo

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
