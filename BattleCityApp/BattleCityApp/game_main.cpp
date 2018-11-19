#include "game.h"
#include "events.h"
#include "screen.h"

Game::Game(sf::RenderWindow &window)
	: window(window), texture(new sf::Texture())
{
	window.setFramerateLimit(30);
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
	GameEvent gE(window);
	GameScreen gS(window);

	// update
	while (window.isOpen())
	{
		gE.UpdateEvent();
		gS.UpdateScreen();

		sf::sleep(sf::milliseconds(25));
	}

	return;
}
