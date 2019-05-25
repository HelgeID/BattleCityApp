#pragma once

#include <SFML/Graphics.hpp>
#include "general.hpp"
#include "ui.hpp"

class NumberFlags : public sf::Drawable
{
public:
	UIFlag* uiFlag{ nullptr };
	UINumber* uiNumber1{ nullptr };
	UINumber* uiNumber2{ nullptr };

	NumberFlags(sf::Texture &texture)
		: uiFlag(new UIFlag(texture)),
		uiNumber1(new UINumber(texture)), uiNumber2(new UINumber(texture))
	{
		const float x(250.f), y(184.f);
		uiFlag->setPosObj(x, y);
		uiNumber1->setPosObj(x, y + 16);
		uiNumber2->setPosObj(x + 8, y + 16);
	}

	~NumberFlags() { delete uiFlag; delete uiNumber1; delete uiNumber2; }
private:
	void draw(sf::RenderTarget& target, sf::RenderStates states) const override
	{
		uiFlag != nullptr ? target.draw(uiFlag->takeObj(), states) : NULL;
		uiNumber1 != nullptr ? target.draw(uiNumber1->takeObj(), states) : NULL;
		uiNumber2 != nullptr ? target.draw(uiNumber2->takeObj(), states) : NULL;
	}
};
