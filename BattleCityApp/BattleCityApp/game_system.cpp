#include "game.h"

//System functions
bool Game::loadTexture()
{
	if (!this->texture->loadFromFile("data/graphics/texture/texture.png", sf::IntRect(0, 0, 400, 256)))
		return EXIT_FAILURE;
	return EXIT_SUCCESS;
}

bool Game::loadIcon()
{
	sf::Image icon;
	if (!icon.loadFromFile("data/graphics/icon/icon.png"))
		return EXIT_FAILURE;
	window.setIcon(176, 176, icon.getPixelsPtr());
	return EXIT_SUCCESS;
}
