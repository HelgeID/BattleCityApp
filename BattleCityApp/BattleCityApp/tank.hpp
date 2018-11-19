#pragma once

#include <SFML/Graphics.hpp>
#include "object.hpp"

enum Color { YELLOW, WHITE, GREEN, RED };
enum Model { modA, modB, modC, modD, modE, modF, modG, modH };
enum Direction { UP, LEFT, DOWN, RIGHT };

class Tank : public Object
{
public:
	Tank(sf::Texture &texture, bool zoom = false) : Object(texture, zoom)
	{
	}

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

		this->setObj(coord.x, coord.y);

		return;
	}

	void reloadTank()
	{
		sf::IntRect rect = this->takeObj().getTextureRect();
		int left = this->takeObj().getTextureRect().left;
		!(left % 32) ?
			this->setObj(rect.left + 16, rect.top) :
			this->setObj(rect.left - 16, rect.top);

		return;
	}
};
