#include "screen.h"

GameScreen::GameScreen(sf::RenderWindow &window)
	: window(window)
{
}

GameScreen::~GameScreen()
{
}

void GameScreen::UpdateScreen()
{
	window.clear();

	window.display();
	return;
}
