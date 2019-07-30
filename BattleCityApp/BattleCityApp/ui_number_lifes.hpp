//FINAL
#pragma once

#include <SFML/Graphics.hpp>
#include "general.hpp"
#include "ui.hpp"

class NumberLifes : public sf::Drawable
{
public:
	UIPlayer* uiPlayer{ nullptr };
	UINumber* uiNumber{ nullptr };

	NumberLifes(sf::Texture &texture, const std::string player_name)
		: uiPlayer(new UIPlayer(texture, player_name)), uiNumber(new UINumber(texture))
	{
		float x, y;
		player_name == "first player" ? x = 248.f, y = 136.f : 0;
		player_name == "second player" ? x = 248.f, y = 160.f : 0;

		uiPlayer->setPosObj(x, y);
		uiNumber->setPosObj(x + 9, y + 9);
	}

	~NumberLifes() { delete uiPlayer; delete uiNumber; }

private:
	void draw(sf::RenderTarget& target, sf::RenderStates states) const override
	{
		uiPlayer != nullptr ? target.draw(uiPlayer->takeObj(), states) : NULL;
		uiNumber != nullptr ? target.draw(uiNumber->takeObj(), states) : NULL;
	}
};
