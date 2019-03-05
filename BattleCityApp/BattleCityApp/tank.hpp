#pragma once

#include <SFML/Graphics.hpp>
#include <time.h>
#include <algorithm>
#include "general.hpp"
#include "object.hpp"
#include "frame.hpp"

#define stringify( name ) # name

enum Color { YELLOW, WHITE, GREEN, RED };
enum Model { playerModA, playerModB, playerModC, playerModD, enemyModA, enemyModB, enemyModC, enemyModD };

#define CoefReloadPlayerModA 8
#define CoefReloadPlayerModB 8
#define CoefReloadPlayerModC 8
#define CoefReloadPlayerModD 8

#define CoefReloadEnemyModA 6
#define CoefReloadEnemyModB 10
#define CoefReloadEnemyModC 6
#define CoefReloadEnemyModD 16

namespace spaceTank
{
	struct Settings
	{
		Color col;
		Model mod;
		Direction dir;

		float step_speed;
		int coef_reload;
		bool bonus;
	};

	struct SettingsShooting
	{
		bool bulletActivFlag;
		sf::Clock clockTank;
		sf::Time timeShooting;
	};

	struct SettingsRDir
	{
		int tankTime_for_dir{ 10 };
		sf::Clock tankClock_for_dir;
	};

	struct SettingsBonus
	{
		sf::Clock clockTank;
		float timeBonus{ 0.f };
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
	int indexTank;
	bool skinTank;

	bool preReverseArr[4] = { false, false, false, false };
	sf::Vector2f prePosReverse;

	bool T{ false }; //on - off
	bool sleeptank{ false };
public:
	Tank(sf::Texture &texture) : Object(texture), Frame("tank"), indexTank(0), skinTank(false)
	{
	}

	Tank(sf::Texture &texture, const char* actorName) : Object(texture), Frame(actorName), indexTank(0), skinTank(false)
	{
	}
	
	void onTank() { T = true; };
	void offTank() { T = false; };
	bool isTank() const { return T; };

	bool& sleepTank() { return sleeptank; };

	Tank& operator=(const Tank& obj)
	{
		if (this == &obj)
			return *this;
		this->~Tank();
		new (this) Tank(obj);
		return *this;
	}

	bool GetSkin() const { return skinTank; }
	void SkinOn() { skinTank = true; return; }
	void SkinOff() { skinTank = false; return; }

	spaceTank::Settings optTank;
	spaceTank::SettingsShooting optTankShooting;
	spaceTank::SettingsRDir optRDir;
	spaceTank::SettingsBonus optBonus;
	spaceTank::MapPosition mapPos;

	void loadTank(Color col, Model mod, Direction dir, bool bonus)
	{
		srand((unsigned)time(NULL));

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

		optTank = { col, mod, dir, 0.f, 0, bonus };
		optTankShooting.bulletActivFlag = false;

		switch (mod)
		{
		case playerModA: optTank.step_speed = 0.f; optTank.coef_reload = CoefReloadPlayerModA; break;
		case playerModB: optTank.step_speed = 0.f; optTank.coef_reload = CoefReloadPlayerModB; break;
		case playerModC: optTank.step_speed = 0.f; optTank.coef_reload = CoefReloadPlayerModC; break;
		case playerModD: optTank.step_speed = 0.f; optTank.coef_reload = CoefReloadPlayerModD; break;
		case enemyModA: optTank.step_speed = 0.3f; optTank.coef_reload = CoefReloadEnemyModA; break;
		case enemyModB: optTank.step_speed = 0.5f; optTank.coef_reload = CoefReloadEnemyModB; break;
		case enemyModC: optTank.step_speed = 0.35f; optTank.coef_reload = CoefReloadEnemyModC; break;
		case enemyModD: optTank.step_speed = 0.15f; optTank.coef_reload = CoefReloadEnemyModD; break;
		}

		return;
	}

	bool switchRW{ false };
	void loadTank_RED_WHITE()
	{
		//todo restart WHITE
		if (switchRW) {
			loadTank(Color::WHITE, optTank.mod, optTank.dir, optTank.bonus);
			switchRW = false;
		}
		else {
			loadTank(Color::RED, optTank.mod, optTank.dir, optTank.bonus);
			switchRW = true;
		}
	}

	void loadIndex(std::vector<Tank>& tank)
	{
		const size_t size = tank.size();
		for (int index(0); index < size; ++index) {
			// -What-
			auto searchResult = std::find_if(tank.begin(), tank.end(),
				[&](const Tank& obj) -> bool { return obj.indexTank == index + 1; });
			if (searchResult == std::end(tank)) {
				// -Where-
				int set_index;
				for (int index(0); index < size; ++index) {
					if (tank[index].indexTank != 0)
						continue;
					set_index = index;
					break;
				}
				// -Set-
				tank[set_index].indexTank = index + 1;
			}
		}
		return;
	}

	void loadIndex(const int indexTank)
	{
		this->indexTank = indexTank;
		return;
	}

	int takeIndex() const
	{
		return indexTank;
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

	Direction ClockWiseDirection(const Direction dir)
	{
		srand((unsigned)time(NULL));

		const bool select = (rand() % 2);

		Direction dirCW;
		switch (dir)
		{
		case UP: select ? dirCW = LEFT : dirCW = RIGHT; break;
		case LEFT: select ? dirCW = DOWN : dirCW = UP; break;
		case DOWN: select ? dirCW = RIGHT : dirCW = LEFT; break;
		case RIGHT: select ? dirCW = UP : dirCW = DOWN; break;
		}

		return dirCW;
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
		//disabled randomness (Random == Reverse)
		//return ReverseDirection(dir);

		srand((unsigned)time(NULL));

		if ((posDirX != (int)round(prePosReverse.x)) | (posDirY != (int)round(prePosReverse.y)))
		{
			prePosReverse.x = posDirX;
			prePosReverse.y = posDirY;
			preReverseArr[3] = preReverseArr[2] = preReverseArr[1] = preReverseArr[0] = false;
		}

		preReverseArr[dir] = incorrectDIR[dir] = true;
		std::vector<int> vecRnd;

		const int size = sizeof incorrectDIR / sizeof *incorrectDIR;
		for (int idx(0); idx < size; ++idx)
			if (!incorrectDIR[idx] && !preReverseArr[idx]) { 
				vecRnd.push_back(idx);
			}
				
		if (vecRnd.size() == 0)
			return ReverseDirection(dir);

		int r_idx = rand() % vecRnd.size();

		if (!(rand() % 2) && dir != DOWN)
			return DOWN;

		return (Direction)vecRnd[r_idx];
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
