#include "game.h"

//System functions
bool Game::setIcon()
{
	sf::Image icon;
	if (!icon.loadFromFile("data/graphics/icon/BattleCityIcon.png"))
		return EXIT_FAILURE;
	window.setIcon(176, 176, icon.getPixelsPtr());
	return EXIT_SUCCESS;
}
