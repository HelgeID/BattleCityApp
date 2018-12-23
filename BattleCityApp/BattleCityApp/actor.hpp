#pragma once

#include <SFML/Graphics.hpp>

#include "general.hpp"
#include "tank.hpp"

class Actor : public Tank
{
private:
	bool presenceflag;
public:
	Actor(sf::Texture &texture,  const bool presenceflag) : Tank(texture), presenceflag(false)
	{
		this->presenceflag = presenceflag;
	}

	const bool Presence() { return presenceflag; }
};
