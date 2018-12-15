#include "game.h"
#include "general.hpp"

//System functions
bool Game::loadTexture()
{
	sf::Image image;
	if (!image.loadFromFile("data/graphics/texture/texture.png"))
		return EXIT_FAILURE;

	image.createMaskFromColor(image.getPixel(0, 0));
	this->texture->loadFromImage(image, sf::IntRect(0, 0, 400, 256));
	
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


void Game::zoomOn()
{
	if (p_zoom == false)
	{
		sf::Vector2u vec;
		vec.x = static_cast<float>(window.getSize().x);
		vec.y = static_cast<float>(window.getSize().y);
		vec.x *= 2; vec.y *= 2;
		window.setSize(vec);
		p_zoom = true;
	}
	return;
}

void Game::zoomOff()
{
	if (p_zoom == true)
	{
		sf::Vector2u vec;
		vec.x = static_cast<float>(window.getSize().x);
		vec.y = static_cast<float>(window.getSize().y);
		vec.x /= 2; vec.y /= 2;
		window.setSize(vec);
		p_zoom = false;
	}
	return;
}
