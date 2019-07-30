//FINAL
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

class UINumber : public Object
{
	unsigned int number{ 0 };
public:
	UINumber(sf::Texture &texture) :
		Object(texture, sf::Vector2f(8.f, 8.f))
	{
		this->clearSpriteObj();
	}

	void UPD_UINumber(const unsigned int number)
	{
		switch (number) {
			case 0: this->setSpriteObj(329, 184); break;
			case 1: this->setSpriteObj(337, 184); break;
			case 2: this->setSpriteObj(345, 184); break;
			case 3: this->setSpriteObj(353, 184); break;
			case 4: this->setSpriteObj(361, 184); break;
			case 5: this->setSpriteObj(329, 192); break;
			case 6: this->setSpriteObj(337, 192); break;
			case 7: this->setSpriteObj(345, 192); break;
			case 8: this->setSpriteObj(353, 192); break;
			case 9: this->setSpriteObj(361, 192); break;
		}
		return;
	}

	void offUINumber() { this->clearSpriteObj(); return; }
	void onUINumber() { UPD_UINumber(number); return; }
};

class UIPlayer : public Object
{
public:
	UIPlayer(sf::Texture &texture, const std::string player_name) :
		Object(texture)
	{
		player_name == "first player" ? this->setSpriteObj(376, 136) : NULL;
		player_name == "second player" ? this->setSpriteObj(376, 160) : NULL;
	}

	void offUIPlayer() { this->clearSpriteObj(); return; }
};

class UIFlag : public Object
{
public:
	UIFlag(sf::Texture &texture) :
		Object(texture)
	{
		this->setSpriteObj(376, 184);
	}
};

class UImPlayer : public Object
{
public:
	UImPlayer(sf::Texture &texture, const std::string player_name) :
		Object(texture, sf::Vector2f(9.f, 9.f))
	{
		player_name == "first player" ? this->setSpriteObj(376, 212) : NULL;
		player_name == "second player" ? this->setSpriteObj(385, 212) : NULL;
	}
};
