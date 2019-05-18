#pragma once
#include <SFML/Graphics.hpp>

#include "general.hpp"
#include "object.hpp"

class UITank : public Object
{
public:
	UITank(sf::Texture &texture) : 
		Object(texture, sf::Vector2f(9.f, 9.f))
	{
		onUITank();
	}

	void onUITank() { this->setSpriteObj(320, 192); return; }
	void offUITank() { this->clearSpriteObj(); return; }
};
