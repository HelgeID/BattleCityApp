#pragma once

#include <SFML/Graphics.hpp>

#include "general.hpp"
#include "tank.hpp"

#define PlayerSpeed (0.4f)
#define PlayerRechargeTime (2.f)

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
		life_counter = 3;
		star_counter = 0;
	}

	bool& Presence() { return presenceflag; }

	void inclife() {
		life_counter++;
		return;
	}

	void declife() {
		if (!life_counter)
			return;
		life_counter--;
		return;
	}

	unsigned int takelife() const { return life_counter; }

	unsigned int& numStar() { return star_counter; }
};
