#pragma once

#include <SFML/Graphics.hpp>
#include "general.hpp"
#include "ui.hpp"

class NumberAllTanks : public sf::Drawable
{
public:
	UITank* uitank;
	UINumber* uiNumber1{ nullptr };
	UINumber* uiNumber2{ nullptr };
	UINumber* uiNumber3{ nullptr };

	NumberAllTanks(sf::Texture &texture)
		: uitank(new UITank(texture)),
		uiNumber1(new UINumber(texture)), uiNumber2(new UINumber(texture)), uiNumber3(new UINumber(texture))
	{
		const float x{ 228.f };
		const float y{ 228.f };
		
		uiNumber1->setPosObj(x, y);
		uiNumber2->setPosObj(x+9, y);
		uiNumber3->setPosObj(x+18, y);
		uitank->setPosObj(x+27, y-1);
	}

	~NumberAllTanks() { delete uitank; delete uiNumber1; delete uiNumber2; delete uiNumber3; }
private:
	void draw(sf::RenderTarget& target, sf::RenderStates states) const override
	{
		uitank != nullptr ? target.draw(uitank->takeObj(), states) : NULL;
		uiNumber1 != nullptr ? target.draw(uiNumber1->takeObj(), states) : NULL;
		uiNumber2 != nullptr ? target.draw(uiNumber2->takeObj(), states) : NULL;
		uiNumber3 != nullptr ? target.draw(uiNumber3->takeObj(), states) : NULL;
	}
};

class NumberTanksForPlayer : public sf::Drawable
{
public:
	UImPlayer* uimPlayer{ nullptr };
	UINumber* uiNumber1{ nullptr };
	UINumber* uiNumber2{ nullptr };
	UINumber* uiNumber3{ nullptr };

	NumberTanksForPlayer(sf::Texture &texture, const std::string player_name)
		: uimPlayer(new UImPlayer(texture, player_name)),
		uiNumber1(new UINumber(texture)), uiNumber2(new UINumber(texture)), uiNumber3(new UINumber(texture))
	{
		float x, y;
		player_name == "first player" ? x = 145.f, y = 228.f : 0;
		player_name == "second player" ? x = 185.f, y = 228.f : 0;

		uiNumber1->setPosObj(x, y);
		uiNumber2->setPosObj(x + 9, y);
		uiNumber3->setPosObj(x + 18, y);
		uimPlayer->setPosObj(x + 27, y-2);
	}

	~NumberTanksForPlayer() { delete uimPlayer; delete uiNumber1; delete uiNumber2; delete uiNumber3; }
private:
	void draw(sf::RenderTarget& target, sf::RenderStates states) const override
	{
		uimPlayer != nullptr ? target.draw(uimPlayer->takeObj(), states) : NULL;
		uiNumber1 != nullptr ? target.draw(uiNumber1->takeObj(), states) : NULL;
		uiNumber2 != nullptr ? target.draw(uiNumber2->takeObj(), states) : NULL;
		uiNumber3 != nullptr ? target.draw(uiNumber3->takeObj(), states) : NULL;
	}
};
