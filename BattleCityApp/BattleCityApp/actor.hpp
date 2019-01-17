#pragma once

#include <SFML/Graphics.hpp>

#include "general.hpp"
#include "tank.hpp"

#define PlayerSpeed (0.5f)
#define PlayerRechargeTime (2.f)

class Player : public Tank
{
private:
	bool presenceflag;
public:
	Player(sf::Texture &texture, const char* actorName, const bool presenceflag) : Tank(texture, actorName), presenceflag(false)
	{
		this->presenceflag = presenceflag;
	}

	bool& Presence() { return presenceflag; }
};
