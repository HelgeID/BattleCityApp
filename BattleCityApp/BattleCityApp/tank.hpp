#pragma once

#include <SFML/Graphics.hpp>
#include <time.h>
#include "general.hpp"
#include "object.hpp"

enum Color { YELLOW, WHITE, GREEN, RED };
enum Model { modA, modB, modC, modD, modE, modF, modG, modH };
enum Direction { UP, LEFT, DOWN, RIGHT };

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

class Tank : public Object
{
public:
	Tank(sf::Texture &texture, bool zoom = false) : Object(texture, zoom)
	{
	}

	Settings optTank;
	MapPosition mapPos;

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
		int dirRnd;
		do {
			dirRnd = rand() % 4;
		} while (dirRnd == dir);	
		return (Direction)dirRnd;
	}
};
