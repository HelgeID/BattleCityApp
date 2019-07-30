//FINAL
#pragma once

#include <SFML/Graphics.hpp>

#include "general.hpp"
#include "tank.hpp"

#define PlayerSpeed (0.65f)
#define PlayerRechargeTime (2.f)
#define PlayerRechargeTime_Star (0.5f)

class Player : public Tank
{
private:
	bool presenceflag;
	unsigned int life_counter;
	unsigned int star_counter;
public:
	Player(sf::Texture &texture, const char* actorName, const bool presenceflag) : Tank(texture, actorName), presenceflag(false)
	{
		this->presenceflag = presenceflag;
		life_counter = 3;//default
		star_counter = 0;//default
	}

	bool& Presence() { return presenceflag; }

	void inclife() {
		if (life_counter < 10)
			life_counter++;
		return;
	}

	void declife() {
		if (!life_counter)
			return;
		life_counter--;
		return;
	}

	unsigned int& numLife() { return life_counter; }

	unsigned int& numStar() { return star_counter; }
};
