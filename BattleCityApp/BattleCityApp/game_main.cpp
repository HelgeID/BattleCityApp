#include "game.h"
#include "general.hpp"
#include "events.h"
#include "field.h"

Game::Game(sf::RenderWindow &window)
	: window(window), texture(new sf::Texture())
{
	window.setFramerateLimit(60);
	loadTexture();
	loadIcon();
}

Game::~Game()
{
	delete texture;
}

//main loader game
void Game::GameLaunch()
{
	// initialization of objects
	GameEvent gEvent(window);
	GameField gField(window, *texture);

	// update
	while (window.isOpen())
	{
		gEvent.UpdateEvent();
		gField.UpdateField();

		//sf::sleep(sf::milliseconds(20));
	}

	return;
}
