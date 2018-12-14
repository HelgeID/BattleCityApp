#pragma once

#include <SFML/Graphics.hpp>
#include <vector>
#include <time.h>
#include "general.hpp"
#include "object.hpp"
#include "frame.hpp"

#define stringify( name ) # name

enum Color { YELLOW, WHITE, GREEN, RED };
enum Model { modA, modB, modC, modD, modE, modF, modG, modH };
enum Direction { UP, LEFT, DOWN, RIGHT };

namespace spaceTank
{
	struct Settings
	{
		Color col;
		Model mod;
		Direction dir;

		float speed;
		int coef_reload;
	};

	struct MapPosition
	{
		int i, j;
	};

	static const char* myDirNames[]
	{
		stringify(UP),
		stringify(LEFT),
		stringify(DOWN),
		stringify(RIGHT)
	};
}

class TankBoom
{
protected:
	int posDirX, posDirY;
	bool incorrectDIR[4];

public:
	virtual void SetBoomCoord(int, int) = 0;
	virtual bool CompareBoomCoord(int, int) = 0;

	void ResetBoomParam()
	{
		posDirX = 0;
		posDirY = 0;
		incorrectDIR[0] = NULL;
		incorrectDIR[1] = NULL;
		incorrectDIR[2] = NULL;
		incorrectDIR[3] = NULL;
	}

	TankBoom() { ResetBoomParam(); }
};

class Tank : public Object, public Frame, public TankBoom
{
public:
	Tank(sf::Texture &texture) : Object(texture), Frame("tank")
	{
	}

	spaceTank::Settings optTank;
	spaceTank::MapPosition mapPos;

	void loadTank(Color col, Model mod, Direction dir)
	{
		const int null(0);
		const int offset(128);
		struct COORD { int x; int y; };

		COORD coord;
		if (col == YELLOW)
			coord = { null , null };
		else if (col == WHITE)
			coord = { offset , null };
		else if (col == GREEN)
			coord = { null , offset };
		else if (col == RED)
			coord = { offset , offset };

		coord.y += mod * 16;
		coord.x += dir * 32;

		this->setSpriteObj(coord.x, coord.y);

		optTank = { col, mod, dir, 0.f, 0};
		switch (mod)
		{
		case modA: optTank.speed = 8000; optTank.coef_reload = 6; break;
		case modB: optTank.speed = 7000; optTank.coef_reload = 6; break;
		case modC: optTank.speed = 6000; optTank.coef_reload = 4; break;
		case modD: optTank.speed = 5000; optTank.coef_reload = 4; break;
		case modE: optTank.speed = 4000; optTank.coef_reload = 3; break;
		case modF: optTank.speed = 3000; optTank.coef_reload = 3; break;
		case modG: optTank.speed = 2000; optTank.coef_reload = 2; break;
		case modH: optTank.speed = 1000; optTank.coef_reload = 2; break;
		default:
			break;
		}

		return;
	}

	void reloadTank()
	{
		sf::IntRect rect = this->takeObj().getTextureRect();
		int left = rect.left;
		!(left % 32) ?
			this->setSpriteObj(rect.left + 16, rect.top) :
			this->setSpriteObj(rect.left - 16, rect.top);

		return;
	}

	Direction ReverseDirection(const Direction dir)
	{
		Direction dirReverse;
		switch (dir)
		{
		case UP: dirReverse = DOWN; goto exit;
		case LEFT: dirReverse = RIGHT; goto exit;
		case DOWN: dirReverse = UP; goto exit;
		case RIGHT: dirReverse = LEFT; goto exit;
		default: goto exit;
		}
	exit:
		return dirReverse;
	}

	Direction RandomReverseDirection(const Direction dir)
	{
		srand((unsigned)time(NULL));
		incorrectDIR[dir] = true;
		std::vector<int> vecRnd;

		const int size = sizeof incorrectDIR / sizeof *incorrectDIR;
		for (int idx(0); idx < size; ++idx)
			if (!incorrectDIR[idx])
				vecRnd.push_back(idx);

		int r_idx = rand() % vecRnd.size();

		if (!(rand() % 2) && dir != DOWN)
			return DOWN;

		return (Direction)vecRnd[r_idx];

		//int dirRnd;
		//do {
		//	dirRnd = rand() % 4;
		//} while (dirRnd == dir);
		//return (Direction)dirRnd;
	}

	void SetBoomCoord(int posX, int posY) override
	{
		posDirX = posX;
		posDirY = posY;
	}

	bool CompareBoomCoord(int posX, int posY) override
	{
		return (posDirX == posX && posDirY == posY);
	}
};
